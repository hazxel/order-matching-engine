# order-matching-engine

### matching strategy
When selling price is lower than buying price, the actual price is dependent on the previous matched price.
 - if previous price is lower than the selling price, the price should be selling price
 - if previous price is higher than the buying price, the price should be buying price
 - if previous price lies between the selling and buying price, the price remain the same as previous 

For example, buyer bids 20, and the seller offers 15. If previous transaction price is 21, then current price should be 20; if previous price is 13, current price shoudl be 15, and if privous price is 18, then it should remain 18. 