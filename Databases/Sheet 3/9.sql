-- A Brasilian enterpreneur is looking to threaten NT’s dominant position in the market and plans to create a new company, Comerciantes do Vento Sul. After bribing you with a lifetime supply of coxinhas, you agree to help him.
-- Identify the top 10% of:
-- - Customers, by the total amount spent
-- - Employees, by the total amount of sales they have generated
-- - Products, by how many times they have been ordered
-- - Suppliers, by the total amount of sales they have generated
-- using views (one, or as many as you need)
-- Create a view that lists the first three of each category, with the following columns:
-- - category (e.g. Customer, Employee, Product, Supplier)
-- - name (e.g. Customer’s company name, Employee’s full name, Product’s name, Supplier’s company name)
-- - total (e.g. total amount spent, total amount of sales, number of orders, total amount of sales)
-- - rank: the rank within the category
-- The view(s) created in the previous step must be used. Order the results by the total amount spent
with top_customers as (
  select company_name as name, total_spent as total, rank
  from (
    select C.company_name, sum(unit_price*quantity*(1-discount)) as total_spent, ntile(10) over top_spending, rank() over top_spending as rank
    from orders O
    join customers C on O.customer_id = C.customer_id
    join order_details OC on O.order_id = OC.order_id
    group by C.customer_id
    window top_spending as (
      order by sum(unit_price*quantity*(1-discount)) desc
    )
  )
  where ntile = 1
),
top_employees as (
  select name, total_sales as total, rank
  from (
    select concat(first_name, last_name) as name, sum(unit_price*quantity*(1-discount)) as total_sales, ntile(10) over top_selling, rank() over top_selling as rank
    from orders O
    join employees E on O.employee_id = E.employee_id
    join order_details OD on OD.order_id = O.order_id
    group by E.employee_id
    window top_selling as (
      order by sum(unit_price*quantity*(1-discount)) desc
    )
  )
  where ntile = 1
),
top_products as (
  select product_name as name, total_sales as total, rank
  from (
    select product_name, count(order_id) as total_sales, ntile(10) over top_selling, rank() over top_selling as rank
    from order_details OD
    join products P on OD.product_id = P.product_id
    group by P.product_id
    window top_selling as (
      order by count(order_id) desc
    )
  )
  where ntile = 1
),
top_suppliers as (
  select company_name as name, total_sales as total, rank
  from (
    select company_name, sum(OD.unit_price*quantity*(1-discount)) as total_sales, ntile(10) over top_selling, rank() over top_selling as rank
    from suppliers S
    join products P on S.supplier_id = P.supplier_id
    join order_details OD on OD.product_id = P.product_id
    group by S.supplier_id
    window top_selling as (
      order by sum(OD.unit_price*quantity*(1-discount)) desc
    )
  )
  where ntile = 1
)
(select 'Customer' as category, *
from top_customers
union
select 'Employee' as category, *
from top_employees
union
select 'Product' as category, *
from top_products
union
select 'Supplier' as category, *
from top_suppliers)
order by total desc