-- Repeat the previous query, but this time:
-- - Filter out all the discontinued products,
-- - Sort the results by the UnitPrice in descending order,
-- - Print highly requested products, i.e., those with more units on order than in stock

select product_name, unit_price
from products
where discontinued = 0 and units_on_order > units_in_stock
order by unit_price desc;