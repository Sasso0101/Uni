-- Northwind Traders’ policy ship orders to customers as soon as possible, but sometimes orders may get delayed or outright lost. List all the orders that have overshot the delivery deadline (Shipped_Date > Required_Date), and list the Customer_ID, Order_ID, and by how many days the order was late

select customer_id, order_id, shipped_date - required_date as delay
from orders
where shipped_date > required_date