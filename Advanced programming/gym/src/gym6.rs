use std::{fmt::Debug, rc::Rc, cell::RefCell};

#[derive(Debug)]
struct TreeNode<T: Clone+PartialOrd> {
    value: T,
    left: Option<Box<TreeNode<T>>>,
    right: Option<Box<TreeNode<T>>>
}

impl<T: Copy+PartialOrd> TreeNode<T> {
    fn new(value: T) -> TreeNode<T> {
        TreeNode {
            value,
            left: None,
            right: None
        }
    }

    fn from_vec(mut vect: Vec<T>) -> TreeNode<T> {
        let mut root = TreeNode::new(vect.remove(0));

        for item in vect {
            Self::insert(item, &mut root);
        }
        root
    }

    fn insert(value: T, root: &mut TreeNode<T>) {
        if value < root.value {
            match &mut root.left {
                Some(left) => Self::insert(value, left),
                None => root.left = Some(Box::new(TreeNode::new(value)))
            }
        } else {
            match &mut root.right {
                Some(right) => Self::insert(value, right),
                None => root.right = Some(Box::new(TreeNode::new(value)))
            }
        }
    }
}

#[derive(Debug)]
struct Car {
    model: String,
    year: u32,
    price: u32,
    rent: bool
}

struct CarDealer {
    cars: Vec<Rc<RefCell<Car>>>
}

struct User {
    car: Option<Rc<RefCell<Car>>>
}

impl CarDealer {
    fn new(cars: Vec<Car>) -> CarDealer {
        CarDealer { cars: cars.into_iter().map(|car| Rc::new(RefCell::new(car))).collect() }
    }

    fn add_car(&mut self, car: Car) {
        self.cars.push(Rc::new(RefCell::new(car)));
    }

    fn print_cars(&self) {
        for car in &self.cars {
            println!("{:?}", car)
        }
    }

    fn rent_user(&self, user: &mut User, model: String) {
        let car = self.cars.iter().find(|car| car.borrow().model == model);
        match car {
            Some(car) => {
                user.car = Some(Rc::clone(car));
                car.borrow_mut().rent = true;
            },
            None => println!("Car not found")
        }
    }

    fn end_rental(&self, user: &mut User) {
        match user.car.as_mut() {
            Some(car) => {
                car.borrow_mut().rent = false;
                user.car = None;
            },
            None => println!("User has no car")
        }
    }
}

trait Sound {
    fn make_sound(&self) -> String;
}

struct Cat;
struct Dog;
impl Sound for Cat {
    fn make_sound(&self) -> String {
        "Miao".to_string()
    }
}
impl Sound for Dog {
    fn make_sound(&self) -> String {
        "Bau".to_string()
    }
}

struct FarmCell {
    element: Box<dyn Sound>,
    next: Option<Rc<RefCell<FarmCell>>>
}

impl Iterator for FarmCell {
    type Item = Rc<RefCell<FarmCell>>;
    fn next(&mut self) -> Option<Self::Item> {
        match &self.next {
            Some(next) => Some(Rc::clone(next)),
            None => None
        }
    }
}

impl FarmCell {
    fn new(obj: impl Sound + 'static) -> FarmCell {
        FarmCell { element: Box::new(obj), next: None }
    }

    fn insert(self, obj: impl Sound + 'static) {
        let last = self.into_iter().last().unwrap();
        let new_el: FarmCell = FarmCell { element: Box::new(obj), next: None };
        last.borrow_mut().next = Some(Rc::new(RefCell::new(new_el)));
    }
}

impl Sound for FarmCell {
    fn make_sound(&self) -> String {
        let mut string = String::from(self.make_sound());

        while last {
            string.push_str(next.borrow().make_sound().as_str());
            last = 
        }
        string
    }
}

#[cfg(test)]
mod tests {
    use crate::gym6::Sound;

    use super::{TreeNode, FarmCell, Dog, Cat};

    #[test]
    fn exercise1() {
        let values = vec![5, 6, 7, 10, 20, 1, 15, 18, 3];
        let root = TreeNode::from_vec(values);
        println!("{:?}", root);
    }

    #[test]
    fn exercise3() {
        let farm = FarmCell::new(Dog);
        farm.insert(Cat);
        println!("{}", farm.make_sound())
    }
}