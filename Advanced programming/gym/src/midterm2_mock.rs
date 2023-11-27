use std::{rc::Rc, cell::RefCell, fmt::Display};

struct BinIter {
    n: Vec<bool>,
}

impl Iterator for BinIter {
    type Item = bool;
    fn next(&mut self) -> Option<Self::Item> {
        self.n.pop()
    }
}

impl BinIter {
    fn new(mut n: i128, l: usize) -> BinIter {
        let mut v = vec![];
        for _ in 0..l {
            let rem = n % 2 != 0;
            n = n / 2;
            v.push(rem);
        }
        v.reverse();
        Self { n: v }
    }
}

#[derive(Debug)]
struct Node<T: PartialEq+Display+Clone> {
    element: T,
    prev: Option<Rc<RefCell<Node<T>>>>,
    next: Option<Rc<RefCell<Node<T>>>>
}
impl<T: PartialEq+Display+Clone> PartialEq for Node<T> {
    fn eq(&self, other: &Self) -> bool {
        self.element.eq(&other.element)
    }
}
impl<T: PartialEq+Display+Clone> Display for Node<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.element)
    }
}

struct List<T: PartialEq+Display+Clone> {
    head: Option<Rc<RefCell<Node<T>>>>,
    tail: Option<Rc<RefCell<Node<T>>>>,
    size: usize
}
impl<T: PartialEq+Display+Clone> List<T> {
    fn new() -> Self {
        Self {
            head: None,
            tail: None,
            size: 0
        }
    }

    fn push(&mut self, element: T) {
        let new_el = Node {
            element,
            prev: None,
            next: self.head.clone()
        };
        let rc = Rc::new(RefCell::new(new_el));
        match self.head.as_ref() {
            None => self.tail = Some(rc.clone()),
            Some(head) => head.borrow_mut().prev = Some(rc.clone())
        }
        self.size += 1;
        self.head = Some(rc.clone());
    }

    fn pop(&mut self) -> Option<T> {
        self.head.clone().and_then(|head| {
            let next = &head.borrow().next;
            match next {
                Some(next) => next.borrow_mut().prev = None,
                None => self.tail = None
            }
            self.head = next.clone();
            self.size -= 1;
            Some(head.borrow().element.clone())
        })
    }

    fn print_list(&self) {
        let mut current = self.head.clone();
        while let Some(current_rc) = current {
            println!("{}", current_rc.borrow().element);
            current = current_rc.borrow().next.clone();
        }
    }

    fn push_back(&mut self, element: T) {
        let new_el = Node {
            element,
            prev: self.tail.clone(),
            next: None
        };
        let rc = Rc::new(RefCell::new(new_el));
        match self.tail.as_ref() {
            None => self.head = Some(rc.clone()),
            Some(tail) => tail.borrow_mut().next = Some(rc.clone())
        }
        self.size += 1;
        self.tail = Some(rc.clone());
    }

    fn pop_back(&mut self) -> Option<T> {
        self.tail.clone().and_then(|tail| {
            let prev = &tail.borrow().prev;
            match prev {
                Some(prev) => prev.borrow_mut().next = None,
                None => self.head = None
            }
            self.tail = prev.clone();
            self.size -= 1;
            Some(tail.borrow().element.clone())
        })
    }
}
mod tests {
    use super::{BinIter, List};

    #[test]
    fn ex1() {
        BinIter::new(4641312598359562305508665788689819792, 128);
        for n in BinIter::new(4641312598359562305508665788689819792, 128) {
            print!("{}", if n { 1 } else { 0 })
        }
        println!();
        for n in BinIter::new(18956403638425120546, 64) {
            print!("{}", if n { 1 } else { 0 })
        }
        println!();
        for n in BinIter::new(15, 4) {
            print!("{}", if n { 1 } else { 0 })
        }
        for n in BinIter::new(1568948940, 16) {
            print!("{}", if n { "*" } else { "_" })
        }
        println!();
        for n in BinIter::new(8978540, 16) {
            print!("{}", if n { "*" } else { "_" })
        }
        println!();
        for n in BinIter::new(375, 9) {
            print!("{}", if n { "*" } else { "_" })
        }
    }

    #[test]
    fn ex2_1() {
        let mut list: List<i32> = List::new();
        list.push(1);
        list.push(2);
        list.push(3);
        list.print_list();
    }

    #[test]
    fn ex2_2() {
        let mut list: List<i32> = List::new();
        list.push(1);
        list.push(2);
        list.push(3);
        list.print_list();
        debug_assert_eq!(list.size, 3);
        debug_assert_eq!(list.head.clone().unwrap().borrow().element, 3);
        debug_assert_eq!(list.tail.clone().unwrap().borrow().element, 1);
    }

    #[test]
    fn ex2_3() {
        let mut list: List<i32> = List::new();
        list.push(1);
        list.push(2);
        list.push(3);
        debug_assert_eq!(list.pop(), Some(3));
        list.print_list();
        debug_assert_eq!(list.pop(), Some(2));
        list.print_list();
        debug_assert_eq!(list.pop(), Some(1));
        list.print_list();
        debug_assert_eq!(list.pop(), None);
        list.print_list();
        debug_assert_eq!(list.size, 0);
        debug_assert_eq!(list.head, None);
        debug_assert_eq!(list.tail, None);
    }

    #[test]
    fn ex2_4() {
        let mut list: List<i32> = List::new();
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        debug_assert_eq!(list.size, 3);
        println!("{}", list.size);
        list.print_list();
        debug_assert_eq!(list.head.clone().unwrap().borrow().element, 1);
        println!("{}", list.head.clone().unwrap().borrow().element);
        debug_assert_eq!(list.tail.clone().unwrap().borrow().element, 3);
        println!("{}", list.tail.clone().unwrap().borrow().element);
    }

    #[test]
    fn ex2_5() {
        let mut list: List<i32> = List::new();
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.print_list();
        debug_assert_eq!(list.pop_back(), Some(3));
        list.print_list();
        debug_assert_eq!(list.pop_back(), Some(2));
        list.print_list();
        debug_assert_eq!(list.pop_back(), Some(1));
        list.print_list();
        debug_assert_eq!(list.pop_back(), None);
        debug_assert_eq!(list.size, 0);
        debug_assert_eq!(list.head, None);
        println!("{:?}", list.head);
        debug_assert_eq!(list.tail, None);
        println!("{:?}", list.tail);
    }
}