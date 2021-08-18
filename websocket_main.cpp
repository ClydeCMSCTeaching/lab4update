#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <functional>
#include <iostream>

#include "Trader/Trader.hpp"

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


void on_open(client *b, websocketpp::connection_hdl hdl) {
    std::string msg2 = "{\n"
                       "    \"type\": \"subscribe\",\n"
                       "    \"product_ids\": [\n"
                       "        \"ETH-USD\"\n"
                       "    ],\n"
                       "    \"channels\": [\n"
                       "        \"level2\"\n"
                       "    ]\n"
                       "}";
    std::cout << "first msg" << std::endl;
    websocketpp::lib::error_code ec;

    b->send(hdl, msg2, static_cast<websocketpp::frame::opcode::value>(1), ec);
    if (ec) {
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    }
}

void on_message(Bot *b, websocketpp::connection_hdl, client::message_ptr msg, client *c) {
    b->message_in(msg->get_payload());
}

/// Verify that one of the subject alternative names matches the given hostname
bool verify_subject_alternative_name(const char *hostname, X509 *cert) {
    STACK_OF(GENERAL_NAME) *san_names = NULL;

    san_names = (STACK_OF(GENERAL_NAME) *) X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);
    if (san_names == NULL) {
        return false;
    }

    int san_names_count = sk_GENERAL_NAME_num(san_names);

    bool result = false;

    for (int i = 0; i < san_names_count; i++) {
        const GENERAL_NAME *current_name = sk_GENERAL_NAME_value(san_names, i);

        if (current_name->type != GEN_DNS) {
            continue;
        }

        char const *dns_name = (char const *) ASN1_STRING_get0_data(current_name->d.dNSName);

        // Make sure there isn't an embedded NUL character in the DNS name
        if (ASN1_STRING_length(current_name->d.dNSName) != strlen(dns_name)) {
            break;
        }
        // Compare expected hostname with the CN
        result = (strcasecmp(hostname, dns_name) == 0);
    }
    sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);

    return result;
}

/// Verify that the certificate common name matches the given hostname
bool verify_common_name(char const *hostname, X509 *cert) {
    // Find the position of the CN field in the Subject field of the certificate
    int common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name(cert), NID_commonName, -1);
    if (common_name_loc < 0) {
        return false;
    }

    // Extract the CN field
    X509_NAME_ENTRY *common_name_entry = X509_NAME_get_entry(X509_get_subject_name(cert), common_name_loc);
    if (common_name_entry == NULL) {
        return false;
    }

    // Convert the CN field to a C string
    ASN1_STRING *common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
    if (common_name_asn1 == NULL) {
        return false;
    }

    char const *common_name_str = (char const *) ASN1_STRING_get0_data(common_name_asn1);

    // Make sure there isn't an embedded NUL character in the CN
    if (ASN1_STRING_length(common_name_asn1) != strlen(common_name_str)) {
        return false;
    }

    // Compare expected hostname with the CN
    return (strcasecmp(hostname, common_name_str) == 0);

}

bool verify_certificate(const char *hostname, bool preverified, boost::asio::ssl::verify_context &ctx) {
    int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());


    if (depth == 0 && preverified) {
        X509 *cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());

        if (verify_subject_alternative_name(hostname, cert)) {
            return true;
        } else if (verify_common_name(hostname, cert)) {
            return true;
        } else {
            return false;
        }
    }

    return preverified;
}

context_ptr on_tls_init(const char *hostname, websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);


        ctx->set_verify_mode(boost::asio::ssl::verify_none);
    } catch (std::exception &e) {
        std::cout << "This erorr I really don't know how to fix. Sorry\n";
    }
    return ctx;
}

int main() {
    client c;

    std::string hostname = "ws-feed.pro.coinbase.com";
    std::string port = "443";

    std::string uri = "wss://" + hostname + ":" + port;

    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);
        c.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        Bot b = Bot();
        c.set_message_handler(bind(&on_message, &b, ::_1, ::_2, &c));
        c.set_tls_init_handler(bind(&on_tls_init, hostname.c_str(), ::_1));
        c.set_open_handler(bind(&on_open, &c, ::_1));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        c.connect(con);


        c.get_alog().write(websocketpp::log::alevel::app, "Connecting to " + uri);

        c.run();
    } catch (websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    }
}