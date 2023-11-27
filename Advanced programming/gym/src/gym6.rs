use std::{fmt::{Debug, Display}, rc::Rc, cell::RefCell};

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
    next: Option<Box<FarmCell>>,
}

impl FarmCell {
    fn new(obj: impl Sound + 'static) -> FarmCell {
        FarmCell { element: Box::new(obj), next: None }
    }

    fn insert(&mut self, obj: impl Sound + 'static) {
        let mut next = &mut self.next;
        loop {
            match next {
                Some(el) => {
                    next = &mut el.next;
                },
                None => break
            }
        }
        let new_el: FarmCell = FarmCell { element: Box::new(obj), next: None };
        *next = Some(Box::new(new_el));
    }
}

impl Sound for FarmCell {
    fn make_sound(&self) -> String {
        let mut string = String::new();
        let mut current = self;
        loop {
            string += current.element.make_sound().as_str();
            match &current.next {
                Some(el) => {
                    current = el;
                },
                None => break
            }
        }
        string
    }
}

#[derive(Debug)]
struct PublicStreetlight {
    id: u32,
    on: bool,
    burned_out: bool
}
impl PublicStreetlight {
    fn new(id: u32, on: bool, burned_out: bool) -> PublicStreetlight {
        PublicStreetlight {
            id, on, burned_out
        }
    }
}
impl Default for PublicStreetlight {
    fn default() -> Self {
        Self { id: 0, on: false, burned_out: false }
    }
}
struct PublicIllumination {
    lights: Vec<PublicStreetlight>
}
impl PublicIllumination {
    fn new() -> PublicIllumination {
        PublicIllumination::default()
    }
}
impl Default for PublicIllumination {
    fn default() -> Self {
        Self { lights: Vec::new() }
    }
}

impl Iterator for PublicIllumination {
    type Item = PublicStreetlight;
    fn next(&mut self) -> Option<Self::Item> {
        let index = self.lights.iter().position(|light| light.burned_out);
        index.and_then(|i| Some(self.lights.remove(i)))
    }
}

trait CompileTimeNode {
    type LeftType: CompileTimeNode;
    type RightType: CompileTimeNode;
    fn is_none(&self) -> bool;
    fn get_left(&self) -> &Self::LeftType;
    fn get_right(&self) -> &Self::RightType;
}
struct NullNode {}
impl CompileTimeNode for NullNode {
    type LeftType = NullNode;
    type RightType = NullNode;
    fn is_none(&self) -> bool {
        true
    }
    fn get_left(&self) -> &Self::LeftType {
        &NullNode{}
    }
    fn get_right(&self) -> &Self::RightType {
        &NullNode{}
    }
}

struct Node<L: CompileTimeNode, R: CompileTimeNode> {
    left: L,
    right: R
}
impl<L, R> CompileTimeNode for Node<L, R>
where L: CompileTimeNode, R: CompileTimeNode {
    type LeftType = L;
    type RightType = R;
    fn is_none(&self) -> bool {
        false
    }
    fn get_left(&self) -> &Self::LeftType {
        &self.left
    }
    fn get_right(&self) -> &Self::RightType {
        &self.right
    }
}
fn count_nodes(root: &impl CompileTimeNode) -> usize {
    let mut count = 0;
    if !root.is_none() {
        count += 1;
        if !root.get_left().is_none() {
            count += count_nodes(root.get_left());
        }
        if !root.get_right().is_none() {
            count += count_nodes(root.get_right());
        }
    }
    count
}

struct EntangledBit {
    value: Rc<RefCell<bool>>
}
impl EntangledBit {
    fn set(&mut self) {
        *self.value.borrow_mut() = true;
    }
    fn reset(&mut self) {
        *self.value.borrow_mut() = false;
    }
    fn get(&self) -> bool {
        *self.value.borrow()
    }
    fn entangle_with(&self, other: &mut Self) {
        other.value = Rc::clone(&self.value);
    }
}
impl Default for EntangledBit {
    fn default() -> Self {
        Self {
            value: Rc::new(RefCell::new(false))
        }
    }
}
impl Display for EntangledBit {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", *self.value.borrow() as u8)
    }
}

#[cfg(test)]
mod tests {
    use crate::gym6::Sound;

    use super::{TreeNode, FarmCell, Dog, Cat, PublicIllumination, PublicStreetlight, Node, NullNode, count_nodes, EntangledBit};

    #[test]
    fn exercise1() {
        let values = vec![5, 6, 7, 10, 20, 1, 15, 18, 3];
        let root = TreeNode::from_vec(values);
        println!("{:?}", root);
    }

    #[test]
    fn exercise3() {
        let mut farm = FarmCell::new(Dog);
        farm.insert(Dog);
        farm.insert(Cat);
        farm.insert(Dog);
        farm.insert(Cat);
        println!("{}", farm.make_sound())
    }

    #[test]
    fn exercise4() {
        let light1 = PublicStreetlight {
            id: 1,
            on: true,
            burned_out: false
        };
        let light2 = PublicStreetlight {
            id: 2,
            on: false,
            burned_out: true
        };
        let light3 = PublicStreetlight {
            id: 3,
            on: true,
            burned_out: true
        };
        let street = PublicIllumination {
            lights: vec![light1, light2, light3]
        };
        for broken in street {
            println!("{:?}", broken);
        }
    }

    #[test]
    fn exercise5() {
        // let c11 = Node {left: NullNode{}, right: NullNode{}};
        let c1 = Node {left: NullNode{}, right: NullNode{}};
        let c21 = Node {left: NullNode{}, right: NullNode{}};
        let c22 = Node {left: NullNode{}, right: NullNode{}};
        let c2 = Node {left: c21, right: c22};
        let root = Node {left: c1, right: c2};
        println!("{}", count_nodes(&root));
    }

    #[test]
    fn exercise6() {
        let mut b1 = EntangledBit::default();
        let mut b2 = EntangledBit::default();
        let mut b3 = EntangledBit::default();
        println!("b1: {b1}, b2: {b2}, b3: {b3}");
        b1.entangle_with(&mut b2);
        b1.set();
        println!("b1: {b1}, b2: {b2}, b3: {b3}");
        b3.set();
        println!("b1: {b1}, b2: {b2}, b3: {b3}");
        b2.entangle_with(&mut b3);
        b2.entangle_with(&mut b1);
        println!("b1: {b1}, b2: {b2}, b3: {b3}");
        b2.reset();
        println!("b1: {b1}, b2: {b2}, b3: {b3}");
    }
}