use std::{cell::RefCell, rc::Rc};

trait MySize {
    fn get_size(&self) -> usize;
}

impl MySize for i32 {
    fn get_size(&self) -> usize {
        4
    }
}

impl MySize for String {
    fn get_size(&self) -> usize {
        self.len()
    }
}
impl<T: MySize> MySize for Vec<T> {
    fn get_size(&self) -> usize {
        let mut size = 0;
        for el in self {
            size += el.get_size()
        }
        size
    }
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

trait Printable {
    fn print(&self);
}

impl Printable for String {
    fn print(&self) {
        println!("{}", self)
    }
}
impl Printable for i32 {
    fn print(&self) {
        if *self >= 0 {
            println!("{}(positive)", self);
        } else {
            println!("{}(negative)", self.abs());
        }
    }
}
fn get_vec() -> Vec<Box<dyn Printable>> {
    vec![]
}
fn print_vec(v: Vec<Box<dyn Printable>>) {
    for el in v {
        el.print()
    }
}

#[derive(Debug)]
struct MasterClock {
    clock: Rc<RefCell<usize>>,
}
#[derive(Debug)]
struct SlaveClock {
    clock: Rc<RefCell<usize>>,
}
impl MasterClock {
    fn new() -> Self {
        Self {
            clock: Rc::new(RefCell::new(0)),
        }
    }
    fn tick(&self) {
        *self.clock.borrow_mut() += 1;
    }
    fn get_slave(&self) -> SlaveClock {
        SlaveClock {
            clock: self.clock.clone(),
        }
    }
}
impl SlaveClock {
    fn get_clock(&self) -> usize {
        *self.clock.borrow()
    }
}

pub mod finance {
    use wallet_1::Wallet as Wallet1;
    pub use wallet_2::Wallet as Wallet2;
    pub mod wallet_1 {
        #[derive(Debug)]
        pub struct Wallet {
            pub euro: f32,
        }
    }

    pub mod wallet_2 {
        #[derive(Debug)]
        pub struct Wallet {
            pub euro: u32,
            pub cents: u8,
        }
    }

    pub fn compare_wallet(first: &Wallet1, second: &Wallet2) -> bool {
        first.euro > (second.euro as f32 + second.cents as f32 / 100 as f32)
    }
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
        Self { head: None, len: 0 }
    }
    fn size(&self) -> i32 {
        self.len
    }
    fn add(&mut self, e: T, p: i32) -> Result<(), String> {
        if p == 0 {
            let first = self.head.take();
            self.head = Some(Box::new(Node {
                elem: e,
                next: first,
            }));
            self.len += 1;
            Ok(())
        } else {
            let mut curr = &mut self.head;
            for i in 0..p - 1 {
                match curr {
                    Some(c) => curr = &mut c.next,
                    None => return Err("wrong position".to_string()),
                }
            }
            match curr {
                Some(c) => {
                    let next = c.next.take();
                    c.next = Some(Box::new(Node { elem: e, next }));
                    self.len += 1;
                }
                None => return Err("wrong position".to_string()),
            }

            Ok(())
        }
    }

    fn prepend(&mut self, e: T) -> Result<(), String> {
        self.add(e, 0)
    }
    fn append(&mut self, e: T) -> Result<(), String> {
        self.add(e, self.len)
    }

    fn get(&self, p: i32) -> Option<&T> {
        if p > self.len - 1 {
            None
        } else {
            let mut curr = &self.head;
            for i in 0..p {
                match curr {
                    Some(c) => curr = &c.next,
                    None => return None,
                }
            }
            match curr {
                Some(el) => return Some(&el.elem),
                None => return None,
            }
        }
    }
}
