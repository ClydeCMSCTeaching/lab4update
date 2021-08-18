# Lab 4
This is the final lab. It has two parts. Each part is worth a lab's worth of credit.

This lab is in C++ is and is designed to illustrate a complex programming problem utilizing real data coming from
websocket. This lab has a lot of moving pieces, and I do not advise moving or changing any file names, the file structure, or touching 
the CMakeLists.txt file or any files in the build folder.

Just in case anyone is confused:

**Disclaimer:**
*this lab is by no means financial advice. I am not giving you financial advice. This is for educational purposes only. I do not advise you to use this code for any real financial activity and
absolve myself and the University of any liability when you modify this code for any non-educational use. I'm not a financial advisor, I'm a computer science
instructor trying to come up with cool assignments that illustrate a complex coding example utilizing websockets.*

# Understanding the Starter Code
This week's starter code is more complex than usual. Given it involves a few dependencies, you must use the linux computers
from UChicago which have OpenSSL, Boost, and CMake properly configured. If you use other systems, I will not be able to help
you with the build system.

For this lab, you are going to want to utilize the build system directly, i.e. *no moving files around*, *no compiling single files*.
We will be using Google's unit testing framework which will allow you to unit test the functions you write for this assignment.

Basic build structure:
```shell
mkdir build
cd build
cmake ..
make all -j 4
./OrderBook # you only need this program for Part I
./runUnitTests # this will run your tests in the test files 
```


# Part 1: Designing a Fast Order Book (150/200)

#### What is an order book?
An **order book**  is a list of buy and sell orders. Order books can be aggregated by price level, i.e. we
group together all orders at a similar price, or order books can be by induvidual orders. We will only deal with
order books aggregated by price level. In order to set up an order book, we need a data structure to hold the data. An order book
aggregated by price level has just three essential units of data, the side, the price level, and the quantity of the asset
available at that price level. For example, consider a very simple market of five people.

- Person A is selling 100 widgets for 10$
- Person B is selling 34 widgets for 9$
- Person C is looking to buy 40 widgets for 8$
- Person D is looking to buy 10 widgets for 7$
- Person E is looking to buy 5 widets for 8$

Our price level aggrigated order book then looks like the following two tables. We have one on the buy side and one
on the sell side




You are going to receive the following snapshot of the order book:
```json
{
    "type": "snapshot",
    "product_id": "BTC-USD",
    "bids": [["10101.10", "0.45054140"], ...],
    "asks": [["10102.55", "0.57753524"], ...]
}
```
This message contains the price levels, the first part of the tuple, and the quantity at that price level, the second part
of the tuple. You need to design an order book data structure that captures these two components, price levels and quantity at
the price level. Your goal is to parse this message into a data structure appropriate for an order book. 

Requirements for an OrderBook (50/150:
- A function which adds orders to the book (i.e. from the snapshot message) (10 points) 
- A function which modifies orders on the book (10 points)
- An Ascii print function which displays the middle of the book, i.e. the lowest 10 sell orders and the highest 10 buy orders. (25 points)
- 5 points for style and efficiency. 

The data the OrderBook should keep is the prices and quantities for all orders on the buy and sell side. The most simple implementation would keep
four sorted vectors, sell prices and quantities, and buy prices and quantities.

**You get a bonus five points if all your order book operators are constant time**

In order to handle changing hands between text messages from the websocket and interpretable data, we will use a message class
in the messages folder which will handle this. You should fill in the details for parsing the snapshot messages. The orderbook should 
take one of these messages and use it to populate it's own data

SnapshotMessage
- Correctly parse messages (80) points
- 10 points for style
- 10 points for efficiency 

Once you write the functions, you then can run the OrderBook test file which will run through the test messages file. 

### Part 1 Program Output
This is what your output should look like based on the file for messages. You do not need to count the number of
`-` or `*` I use. (I use 50 `*` to represent the maximum quantity, but feel free to use more of less.). Recall, the output
should show us the current state of the order book. We have the top 10 lowest asking (selling) offers, and their quantities shown.
Then we have the market spread, which is the lowest selling price minus the highest asking (buying, bidding) price. Then we show
the top 10 orders for the buy order. 

```shell
> ./OrderBook messages.txt
running with /Users/austin/CLionProjects/Lab4/messages.txt
------------------------------------------------------------------------------
3071.61 |***********
3071.47 |******
3071.33 |**********************
3071.32 |*****************************************
3071.01 |*
3070.87 |*************************
3070.75 |*********************
3070.74 |*******************************
3070.73 |******
3070.48 |***********************
------------------------- middle market (spread: 0.02)-------------------------
3070.46 |***************************************************
3070.45 |**********************************
3070.00 |*
3069.95 |*
3069.94 |*
3069.78 |*************************
3069.76 |**********
3069.75 |***************************
3069.66 |*********************
3069.62 |*
------------------------------------------------------------------------------
-> * = 0.07 coins
```


### How you should approach this

1. Start by writing the order book and testing it. You can test it by manually adding in some orders and making up some things and running the gtest suite. 
2. Write the message class for a `snapshot` which should have a parse function which takes a string and parses it into the correct data
3. Write a function to read in the text file, the whole thing, as a string. 

# Part 2: Keeping Your Book Updated and Real-Time Analysis (50/200)
Once the order book is updated, you will then receive messages which allow you to update the order book in real time. Do not get
overwhelmed, but if you look at the `websocket_main.cpp`, there is already code there which connects Coinbase's websocket API
and subscribes to their ETH-USD level 2 updates. After the websocket sends you the whole order book, they then will send you update messages
which lok like the following:

```json
{
  "type": "l2update",
  "product_id": "BTC-USD",
  "time": "2019-08-14T20:42:27.265Z",
  "changes": [
    [
      "buy",
      "10101.80000000",
      "0.162567"
    ]
  ]
}
```
You will first need to write a parser for these `l2update` messages. 


#### How you should approach this:
1. Based on the data you see for an `l2update` message, write an `l2updateMessage` class. Be sure to also write the parser for it.
2. In the `Trader.hpp` code you need to write a dispatcher. A dispatcher takes all websocket messages, and determines the type of it 
   (i.e the literal word written in the "type" field of the message). Then the dispatcher will create the correct `Message` object for that type, and 
   *crucially* decide what it should do with that message (should it initialize the order book? should it update the order book? should it just ignore it?). You are expected to handle 
   `l2update`, `snapshot`, and `subscriptions` (though you can ignore those messages!).
3. Update your message dispatcher to check for a type that is `l2update`.
4. Write a parser for the file `all_messages.txt` which simulates running the real websocket.
5. Once you've tested your code with the simulated websocket feed, you can connect your Trader class to the websocket_main.cpp file, and run it.
   This is a live connection to Coinbase's market exchange data. Congrats!
6. Your `Trader` class should call the print order book function every 15 seconds and update the order book. The timing does not need to be exact, 
for instance, you can measure the time elasped by just looking at the timestamp on the orders (thus go back and add it to your message object), and if a message
   comes with a time 15 seconds since you last printed, print the new book. 

- Aspect 1: 8 points
- Aspect 2: 8 points
- Aspect 3: 3 points
- Aspect 4: 8 points
- Aspect 5: 8 points 
- Aspect 6: 3 points
- Code style and efficiency: 12

Total = 50.

# Notes/Errata 
### Extra Credit Leaderboard
If you produce a runnable executable that compiles and conforms to the output standards, we will time your code
for reading a different `messages.txt` file from a different time. If you have one of the three fastest, you will get `25` 
extra credit points. If you have one of the top six fastest, but were not in the top three, you will get 15 extra credit points.
If you were in the top 15 fastest and were not in the top six or top three, you will get 5 extra credit points. If there is a tie, we will choose another day as well, and take the average of the times.
This refers only to part 1.


### Extra Credit Unit Testing
If you unit test every single function you write, I will add 15 extra credit points. Clearly label all unit tests in the corresponding test file.
You can run your unit tests by going to your build folder, and running Testing/runUnitTests