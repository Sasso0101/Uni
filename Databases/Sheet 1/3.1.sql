-- Northwind Traders is considering expanding its operations in developing countries. First, provide an ordered list (by the amount of money spent) of the countries where the company has customers, the number of orders in each country, and the total amount of money spent in each one
select country, count(distinct order_id) as order_cnt, sum(unit_price*quantity*(1-discount)) as total
from orders
natural join order_details
natural join customers
group by country
order by total