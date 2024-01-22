use std::cell::RefCell;
use std::fmt::{Debug, Display, Formatter};
use std::ops::Deref;
use std::rc::Rc;

trait Doublable<T> {
    fn gimme_double(&self) -> Self;
}

impl<T> Doublable<T> for i32 {
    fn gimme_double(&self) -> Self {
        self * 2
    }
}

impl<T> Doublable<T> for String {
    fn gimme_double(&self) -> Self {
        self.repeat(2)
    }
}

fn printdouble<T: Doublable<T> + Debug>(el: T) {
    println!("{:?}", el.gimme_double());
}

struct Wrapper {
    v: Vec<i32>,
}

struct IteratorWrapper<'a> {
    vec: &'a Vec<i32>,
    index: usize,
}

impl<'a> Iterator for IteratorWrapper<'a> {
    type Item = i32;
    fn next(&mut self) -> Option<Self::Item> {
        self.index += 2;
        self.vec.get(self.index - 2).copied()
    }
}

impl Wrapper {
    pub fn iter(&self) -> IteratorWrapper {
        IteratorWrapper {
            vec: &self.v,
            index: 1,
        }
    }
}

trait Appendable {
    fn append(&self, original: &mut String);
}

impl Appendable for String {
    fn append(&self, original: &mut String) {
        original.push_str(self);
    }
}

impl Appendable for u32 {
    fn append(&self, original: &mut String) {
        original.push_str(&self.to_string());
    }
}

fn get_vec() -> Vec<Box<dyn Appendable>> {
    vec![]
}

fn concat(vec: &Vec<Box<dyn Appendable>>) -> String {
    let mut fin = String::new();
    for mut el in vec {
        el.append(&mut fin);
    }
    fin
}

#[derive(Debug)]
struct SharedCommunications {
    message: Rc<RefCell<Option<String>>>,
}

impl SharedCommunications {
    fn new() -> Self {
        SharedCommunications {
            message: Rc::new(RefCell::new(None)),
        }
    }

    fn new_form(other: &Self) -> Self {
        SharedCommunications {
            message: other.message.clone(),
        }
    }

    fn send(&mut self, message: String) -> Result<(), ()> {
        if self.message.borrow().is_some() {
            Err(())
        } else {
            *self.message.borrow_mut() = Some(message);
            Ok(())
        }
    }

    fn receive(&mut self) -> Option<String> {
        let mut rr = self.message.borrow_mut();
        match rr.as_mut() {
            Some(message) => {
                let message = message.clone();
                *rr = None;
                Some(message.clone())
            }
            None => None,
        }
    }
}

mod point_2d {
    #[derive(Debug)]
    pub struct Point {
        pub x: f32,
        pub y: f32,
    }
}

mod point_3d {
    use crate::midterm2_2023_1::point_2d;

    #[derive(Debug)]
    pub struct Point {
        pub x_y: point_2d::Point,
        pub z: f32,
    }
}

mod util {
    pub use crate::midterm2_2023_1::point_2d::Point as Point2D;
    pub use crate::midterm2_2023_1::point_3d::Point as Point3D;

    pub fn _3d_to_2d(td: Point3D) -> Point2D {
        td.x_y
    }
}

#[derive(Debug)]
pub struct Content {
    pub i: i32,
    pub s: String,
}

impl Content {
    pub fn new(i: i32, s: String) -> Content {
        Content { i, s }
    }
}
impl Display for Content {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "I: {}, S: {}", self.i, self.s)
    }
}

#[derive(Debug)]

struct Node<T> {
    elem: T,
    left: TreeLink<T>,
    center: TreeLink<T>,
    right: TreeLink<T>,
}

impl<T> Node<T> {
    pub fn new(elem: T) -> Node<T> {
        Node {
            elem,
            left: None,
            center: None,
            right: None,
        }
    }
}

fn sum_path(node: &Option<Box<Node<i32>>>, total: &mut i32, count: &mut usize) {
    if *total > 0 {
        if let Some(node) = node {
            *total -= node.elem.to_string().len() as i32;
            *count += 1;
            sum_path(&node.left, total, count);
            sum_path(&node.center, total, count);
            sum_path(&node.right, total, count);
        }
    }
}

#[derive(Debug)]
pub struct Tree<T> {
    root: TreeLink<T>,
}

type TreeLink<T> = Option<Box<Node<T>>>;

impl<T> Tree<T> {
    fn new() -> Self {
        Tree { root: None }
    }
}

impl Tree<i32> {
    fn add(&mut self, num: i32) -> Option<()> {
        let mut curr_opt = &mut self.root;
        while let Some(curr) = curr_opt {
            match num.to_string().len().cmp(&curr.elem.to_string().len()) {
                std::cmp::Ordering::Less => curr_opt = &mut curr.left,
                std::cmp::Ordering::Equal => curr_opt = &mut curr.center,
                std::cmp::Ordering::Greater => curr_opt = &mut curr.right,
            }
        }
        let _ = curr_opt.insert(Box::new(Node {
            elem: num,
            left: None,
            center: None,
            right: None,
        }));
        Some(())
    }

    fn sum_path(&self, mut total: i32) -> usize {
        let mut count = 0;
        sum_path(&self.root, &mut total, &mut count);
        count
    }
}

fn main() {}

#[cfg(test)]
mod tests {
    use crate::midterm2_2023_1::{
        concat, get_vec, point_3d, printdouble, Content, SharedCommunications, Tree, Wrapper,
    };

    #[test]
    fn ex6() {
        let mut t = Tree::new();
        for i in [100, 11, 83245, 192345, 450, 452, 453] {
            t.add(i).unwrap()
        }
        let res = t.sum_path(11);
        println!("{}",res);
        let res = t.sum_path(100);
        println!("{}",res);
        let res = t.sum_path(16);
        println!("{}",res);
    }

    #[test]
    fn ex4() {
        let mut c1 = SharedCommunications::new();
        let mut c2 = SharedCommunications::new_form(&c1);

        println!("{:?}", c1.send("hello".to_owned()));
        println!("{:?}", c2.receive());
        println!("{:?}", c1.send("hello2".to_owned()));
        println!("{:?}", c2.receive());
        println!("{:?}", c1.send("hello3".to_owned()));
        println!("{:?}", c2.receive());
        println!("{:?}", c1.send("hello4".to_owned()));
        println!("{:?}", c2.receive());
    }

    #[test]
    fn ex1() {
        let x = 5;
        printdouble(x);
        let s = "what".to_string();
        println!("normal s: {:?}", s);
        printdouble(s);
        let y = 8;
        printdouble(y);

        let x = 10;
        printdouble(x);
        let s = "who".to_string();
        println!("normal s: {:?}", s);
        printdouble(s);
        let y = 8;
        printdouble(y);
    }

    #[test]
    fn ex2() {
        let w = Wrapper {
            v: vec![1, 2, 3, 4, 5, 6, 7, 8],
        };
        let mut iw = w.iter();
        println!("first: {}", iw.next().unwrap());
        for el in iw {
            println!("evens: {}", el);
        }

        let w = Wrapper {
            v: vec![11, 12, 13, 14, 15, 16, 17, 18],
        };
        let mut iw = w.iter();
        println!("first: {}", iw.next().unwrap());
        for el in iw {
            println!("evens: {}", el);
        }
    }

    #[test]
    fn ex3() {
        let mut v = get_vec();
        v.push(Box::new(358));
        println!("{}", concat(&v));
        v.push(Box::new("hi there".to_string()));
        println!("{}", concat(&v));
        v.push(Box::new(87));
        println!("{}", concat(&v));
        v.push(Box::new("general kenobi".to_string()));
        println!("{}", concat(&v));
        assert_eq!(concat(&v), "358hi there87general kenobi");
    }
}
