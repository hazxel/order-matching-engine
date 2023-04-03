# Order Matching Engine

### Introduction
Here is a simple implement of an order matching engine that can process buy and sell orders for different instruments (BTC/ETH/USDT). The engine is able to handle a high volume of orders efficiently. 

The engine supports the following operations:
- Place a buy or sell order with a specified instrument and quantity at a given price.
- Retrieve the current order book for a specific instrument.
- Retrieve the executed trades for a specific instrument.
- Cancel an existing order.

Once a incoming order is inserted to the orderbook, an acknowledgment message will be sent to an output queue, and the matching engine checks if it can be matched with existing orders in the order book based on their price and instrument. 
If a match is found, the orders should be executed, and trade messages should be sent to the output queue. The residual quantities of the order will be updated.
  


### How to run
 - to make the project: `make`
 - to run a simple case: `./bin/run`

### Matching strategy
When selling price is lower than buying price, the actual price is dependent on the previous matched price.
 - if previous price is lower than the selling price, the price should be selling price
 - if previous price is higher than the buying price, the price should be buying price
 - if previous price lies between the selling and buying price, the price remain the same as previous 

For example, buyer bids 20, and the seller offers 15. If previous transaction price is 21, then current price should be 20; if previous price is 13, current price shoudl be 15, and if privous price is 18, then it should remain 18. 