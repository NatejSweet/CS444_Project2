# CS444_Project2
# Reservation Brokers


## Building

Command Line 
* `make` to build. An executable called `reservations` will be produced.
* usage: ./reservations seat_count broker_count xaction_count

VS Code:

* The default build task runs `make`.



## Files

* `reservations.c` : main source file

## Data

* `seat_count`: This is the total number of seats
* `broker_count`: This is the number or brokers, aka threads making reservations
* `seat_taken`: This is an array of the seats in use
* `transaction_count`: This is the number of transactions that have taken place
* `seat_taken_count`: This is the number of seats reserverved

## Functions

* `main()` - sets up and starts threads on seat_broker function
* `seat_broker()`for the number of transactions, try to reserve a seat, if taken, print that theres an issue
    * `reserve_seat()` - reserves a seat
    * `free_seat() - unreserves a seat
    * `verify_seat_count()` - verfifies that the seat count (open) is correct. 
* `is_free()` - checks to see if passed seat is free