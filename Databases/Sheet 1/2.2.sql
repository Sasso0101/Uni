-- Expand the previous query and investigate what are the most offending shipment companies: list the CompanyName and the number of orders they have shipped late, plus the average delay in days. Sort the results by the number of late orders in descending order

select company_name, count(order_id) as delayed_orders, avg(shipped_date - required_date) as avg_delay
from orders
join shippers on orders.ship_via = shippers.shipper_id
where shipped_date > required_date
group by shipper_id
order by delayed_orders desc