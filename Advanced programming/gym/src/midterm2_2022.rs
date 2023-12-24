use std::{
    cell::RefCell,
    fmt::{Debug, Formatter},
    ops::Deref,
    rc::Rc,
    slice::Iter,
};

trait Doublable
where
    Self: Sized,
{
    fn gimme_double(&self) -> Self;
}

impl Doublable for i32 {
    fn gimme_double(&self) -> Self {
        self * 2
    }
}

impl Doublable for String {
    fn gimme_double(&self) -> Self {
        self.repeat(2)
    }
}

fn printdouble<T: Doublable + Debug>(doublable: T) {
    println!("{:?}", doublable.gimme_double())
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

fn basicbox_sum(vec: Vec<String>) -> Vec<usize> {
    let mut res = vec![];
    let mut sum = 0;
    for s in vec {
        res.push(s.len());
        sum += s.len();
    }
    res.push(sum);
    res
}

#[derive(Debug)]
pub struct List<T> {
    head: Link<T>,
    len: i32,
}
type Link<T> = Option<Box<Node<T>>>;
#[derive(Debug)]
struct Node<T> {
    elem: T,
    next: Link<T>,
}
#[derive(Debug)]
pub struct Content {
    s: String,
    b: bool,
    i: i32,
}
impl Content {
    pub fn new_with(s: String, b: bool, i: i32) -> Content {
        return Content { s, b, i };
    }
}
impl<T> List<T> {
    fn new() -> Self {
        List { head: None, len: 0 }
    }
    fn size(&self) -> i32 {
        self.len
    }

    fn add(&mut self, element: T, position: usize) -> Result<(), String> {
        if position > self.len as usize {
            return Err("wrong position".to_string());
        }
        if position == 0 {
            self.head = Some(Box::new(Node {
                elem: element,
                next: self.head.take(),
            }));
        } else {
            let mut current_node = self.head.as_mut().unwrap();
            let mut current = 0;
            while current != position - 1 {
                current_node = current_node.next.as_mut().unwrap();
                current += 1;
            }
            current_node.next = Some(Box::new(Node {
                elem: element,
                next: current_node.next.take(),
            }));
        }
        self.len += 1;
        Ok(())
    }

    fn prepend(&mut self, element: T) {
        self.head = Some(Box::new(Node {
            elem: element,
            next: self.head.take(),
        }));
        self.len += 1;
    }
    fn append(&mut self, element: T) {
        let len = self.len as usize;
        let _ = self.add(element, len);
    }

    fn get<'a>(&'a self, position: usize) -> Option<&'a T> {
        if position as i32 > self.len - 1 {
            return None;
        }
        let mut current_node = self.head.as_ref().unwrap();
        let mut current = 0;
        while current != position {
            current_node = current_node.next.as_ref().unwrap();
            current += 1;
        }
        return Some(&current_node.elem);
    }
}

type NodeRef<T> = Rc<RefCell<Node1<T>>>;
struct Node1<T> {
    inner_value: T,
    adjacent: Vec<NodeRef<T>>,
}
impl<T: Debug> Debug for Node1<T> {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "iv: {:?}, adj: {}",
            self.inner_value,
            self.adjacent.len()
        )
    }
}
#[derive(Debug)]
struct Graph<T> {
    nodes: Vec<NodeRef<T>>,
}
impl<T> Graph<T> {
    fn new() -> Self {
        Self { nodes: vec![] }
    }
}
impl<T: SameBool + PartialOrd> Graph<T> {
    fn add_node(&mut self, node: T) {
        let rc = Rc::new(RefCell::new(Node1 { inner_value: node, adjacent: vec![] }));
        for el in &self.nodes {
            if el.borrow().inner_value.samebool(&rc.borrow().inner_value) {
                el.borrow_mut().adjacent.push(rc.clone());
            }
            if el.borrow().inner_value < rc.borrow().inner_value {
                rc.borrow_mut().adjacent.push(el.clone());
            }
        }
        self.nodes.push(rc.clone());
    }
}
pub trait SameBool {
    fn samebool(&self, other: &Self) -> bool;
}
#[derive(Debug)]
pub struct Content1 {
    pub i: i32,
    pub b: bool,
}
impl Content1 {
    pub fn new_with(i: i32, b: bool) -> Content1 {
        Content1 { i, b }
    }
}
impl SameBool for Content1 {
    fn samebool(&self, other: &Self) -> bool {
        self.b == other.b
    }
}
impl PartialEq for Content1 {
    fn eq(&self, other: &Self) -> bool {
        self.i == other.i
    }
}
impl PartialOrd for Content1 {
    fn ge(&self, other: &Self) -> bool {
        self.i >= other.i
    }
    fn gt(&self, other: &Self) -> bool {
        self.i > other.i
    }
    fn le(&self, other: &Self) -> bool {
        self.i <= other.i
    }
    fn lt(&self, other: &Self) -> bool {
        self.i < other.i
    }
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.i.partial_cmp(&other.i)
    }
}

mod tests {
    use crate::midterm2_2022::{
        basicbox_sum, printdouble, Content, Content1, Graph, List, Node, Wrapper, SameBool,
    };

    #[test]
    pub fn ex1_1() {
        let x = 5;
        printdouble(x);
        let s = "what".to_string();
        println!("normal s: {:?}", s);
        printdouble(s);
        let y = 8;
        printdouble(y);
    }

    #[test]
    pub fn ex1_2() {
        let x = 10;
        printdouble(x);
        let s = "who".to_string();
        println!("normal s: {:?}", s);
        printdouble(s);
        let y = 8;
        printdouble(y);
    }

    #[test]
    pub fn ex2_1() {
        let w = Wrapper {
            v: vec![1, 2, 3, 4, 5, 6, 7, 8],
        };
        let mut iw = w.iter();
        println!("first: {}", iw.next().unwrap());
        for el in iw {
            println!("evens: {}", el);
        }
    }

    #[test]
    pub fn ex2_2() {
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
    pub fn ex3_1() {
        let s = vec!["asd".to_string(), "where".to_string(), "what".to_string()];
        println!("boxed s: {:?}", basicbox_sum(s));
    }
    #[test]
    pub fn ex3_2() {
        let s = vec![
            "nope".to_string(),
            "game".to_string(),
            "bananas".to_string(),
        ];
        println!("boxed s: {:?}", basicbox_sum(s));
    }

    #[test]
    pub fn ex4_1() {
        let l: List<i32> = List::new();
        println!("{:?}", l);
        assert_eq!(l.size(), 0);
        let l = List {
            head: Some(Box::new(Node {
                elem: 4,
                next: None,
            })),
            len: 1,
        };
        assert_eq!(l.size(), 1);
        let s: String = format!("{:?}", l);
        assert_eq!(s.contains("Vec"), false);
    }
    #[test]
    pub fn ex4_2() {
        let l: List<i32> = List::new();
        println!("{:?}", l);
        assert_eq!(l.get(0), None);
        assert_eq!(l.get(1), None);
        let l = List {
            head: Some(Box::new(Node {
                elem: 4,
                next: None,
            })),
            len: 1,
        };
        assert_eq!(l.get(0), Some(&4));
        let s: String = format!("{:?}", l);
        assert_eq!(s.contains("Vec"), false);
    }
    #[test]
    pub fn ex4_3() {
        let elem0 = Content::new_with("test".to_string(), true, 2);
        let elem1 = Content::new_with("what".to_string(), true, 2);
        let elem2 = Content::new_with("this".to_string(), false, 8);
        let elem3 = Content::new_with("dope".to_string(), true, 18);
        let mut l: List<Content> = List::new();
        println!("{:?}", l.add(elem0, 1));
        println!("{:?}", l);
        l.add(elem1, 0);
        println!("{:?}", l);
        l.add(elem2, 1);
        println!("{:?}", l);
        l.add(elem3, 2);
        println!("{:?}", l);
        let elem4 = Content::new_with("nope".to_string(), false, 1);
        l.add(elem4, 4);
        println!("{:?}", l);
        let s: String = format!("{:?}", l);
        assert_eq!(s.contains("Vec"), false);
    }
    #[test]
    pub fn ex4_4() {
        let elem0 = Content::new_with("test".to_string(), true, 2);
        let elem1 = Content::new_with("what".to_string(), true, 2);
        let elem2 = Content::new_with("this".to_string(), false, 8);
        let elem3 = Content::new_with("dope".to_string(), true, 18);
        let mut l: List<Content> = List::new();
        l.prepend(elem0);
        println!("{:?}", l);
        l.prepend(elem1);
        println!("{:?}", l);
        l.prepend(elem2);
        println!("{:?}", l);
        let s: String = format!("{:?}", l);
        assert_eq!(s.contains("Vec"), false);
    }
    #[test]
    pub fn ex4_5() {
        let elem0 = Content::new_with("test".to_string(), true, 2);
        let elem1 = Content::new_with("what".to_string(), true, 2);
        let elem2 = Content::new_with("this".to_string(), false, 8);
        let elem3 = Content::new_with("dope".to_string(), true, 18);
        let mut l: List<Content> = List::new();
        l.append(elem0);
        println!("{:?}", l);
        l.append(elem1);
        println!("{:?}", l);
        l.append(elem2);
        println!("{:?}", l);
        let s: String = format!("{:?}", l);
        assert_eq!(s.contains("Vec"), false);
    }

    #[test]
    pub fn ex5_1() {
        let mut el1 = Content1 { i: 10, b: true };
        let mut el2 = Content1 { i: 11, b: true };
        let mut el3 = Content1 { i: 11, b: false };
        assert_eq!(el1 < el2, true);
        assert_eq!(el2 < el1, false);
        assert_eq!(el2 == el3, true);
    }
    #[test]
    pub fn ex5_2() {
        let mut el1 = Content1 { i: 10, b: true };
        let mut el2 = Content1 { i: 11, b: true };
        let mut el3 = Content1 { i: 11, b: false };
        assert_eq!(el1.samebool(&el2), true);
        assert_eq!(el1.samebool(&el3), false);
    }
    #[test]
    pub fn ex5_3() {
        let mut g: Graph<Content1> = Graph::new();
        println!("{:?}", g);
    }
    #[test]
    pub fn ex5_4() {
        let mut el1 = Content1 { i: 10, b: true };
        let mut el2 = Content1 { i: 11, b: true };
        let mut el3 = Content1 { i: 12, b: false };
        let mut g = Graph::new();
        println!("{:?}", g);
        g.add_node(el1);
        println!("{:?}", g);
        g.add_node(el2);
        println!("{:?}", g);
        g.add_node(el3);
        println!("{:?}", g);

        let mut el1 = Content1 { i: 10, b: true };
        let mut el2 = Content1 { i: 8, b: false };
        let mut g = Graph::new();
        println!("{:?}", g);
        g.add_node(el1);
        println!("{:?}", g);
        g.add_node(el2);
        println!("{:?}", g);

        let mut el1 = Content1 { i: 10, b: true };
        let mut el2 = Content1 { i: 11, b: true };
        let mut el3 = Content1 { i: 12, b: true };
        let mut g = Graph::new();
        println!("{:?}", g);
        g.add_node(el1);
        println!("{:?}", g);
        g.add_node(el2);
        println!("{:?}", g);
        g.add_node(el3);
        println!("{:?}", g);

        let mut el1 = Content1 { i: 10, b: true };
        let mut el2 = Content1 { i: 9, b: false };
        let mut el3 = Content1 { i: 8, b: true };
        let mut g = Graph::new();
        println!("{:?}", g);
        g.add_node(el1);
        println!("{:?}", g);
        g.add_node(el2);
        println!("{:?}", g);
        g.add_node(el3);
        println!("{:?}", g);
    }
}
