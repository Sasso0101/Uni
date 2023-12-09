use std::{rc::Rc, cell::RefCell, fmt::{Display, Debug}, collections::LinkedList, ops::Add};

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

#[derive(Debug)]
struct List<T: PartialEq+Display+Clone> {
    head: Option<Rc<RefCell<Node<T>>>>,
    tail: Option<Rc<RefCell<Node<T>>>>,
    size: usize
}
impl<T: PartialEq+Display+Clone> PartialEq for List<T> {
    fn eq(&self, other: &Self) -> bool {
        if self.size == 0 && other.size == 0 {
            return true;
        }
        if self.size != other.size {
            return false;
        }
        if let Some(cur1) = self.head.clone() {
            if let Some(cur2) = other.head.clone() {
                if *cur1.borrow() == *cur2.borrow() {
                    return Self {
                        head: cur1.borrow().next.clone(),
                        tail: self.tail.clone(),
                        size: self.size - 1
                    } == Self {
                        head: cur2.borrow().next.clone(),
                        tail: other.tail.clone(),
                        size: other.size - 1
                    };
                }
            }
        }
        return false;
    }
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

trait Task {
    fn execute(&self) -> usize;
}
struct SumTask {
    n1: usize,
    n2: usize
}

impl SumTask {
    fn new(n1: usize, n2: usize) -> Self{
        Self { n1, n2 }
    }
}
impl Task for SumTask {
    fn execute(&self) -> usize {
        self.n1 + self.n2
    }
}
struct LenTask {
    s: String
}
impl LenTask {
    fn new(s: String) -> Self {
        Self { s }
    }
}
impl Task for LenTask {
    fn execute(&self) -> usize {
        self.s.len()
    }
}
struct Executer {
    executer_queue: Rc<RefCell<LinkedList<Box<dyn Task>>>>
}
impl Executer {
    fn execute_task(&self) -> Option<usize> {
        self.executer_queue.borrow_mut().pop_front().and_then(|task| {
            Some(task.execute())
        })
    }
}
struct Tasker {
    executer: Rc<RefCell<Executer>>
}
impl Tasker {
    fn new() -> Self {
        Self {
            executer: Rc::new(RefCell::new(Executer { executer_queue: Rc::new(RefCell::new(LinkedList::new())) }))
        }
    }

    fn get_tasker(&self) -> Tasker {
        Tasker {
            executer: self.executer.clone()
        }
    }

    fn get_executer(&self) -> Executer {
        Executer { executer_queue: self.executer.borrow().executer_queue.clone() }
    }

    fn schedule_task(&mut self, task: Box<dyn Task>) {
        self.executer.borrow().executer_queue.borrow_mut().push_back(task);
    }
}

mod odd_module {
    pub const CONSTANT: i32 = 123;
}
mod even_module {
    pub const CONSTANT: i32 = 246;
}
mod getter_function {
    use super::{odd_module, even_module};

    pub fn get_constant(value: u32) -> i32 {
        if value % 2 != 0 {
            odd_module::CONSTANT
        } else {
            even_module::CONSTANT
        }
    }
}

trait CloneAndDouble: Clone+Add {
    fn clone_and_double(&self) -> Self::Output {
        self.clone().add(self.clone())
    }
}
impl<T: Clone+Add> CloneAndDouble for T {}

trait Unknown {
    fn serialize(&self) -> String;
}
impl Unknown for i32 {
    fn serialize(&self) -> String {
        self.to_string()
    }
}
impl Unknown for String {
    fn serialize(&self) -> String {
        self.to_string()
    }
}
impl<T: Debug> Unknown for Vec<T> {
    fn serialize(&self) -> String {
        format!("{:?}", self)
    }
}

fn get_vec() -> Vec<Box<dyn Unknown>> {
    Vec::new()
}

fn print_vec(v: &Vec<Box<dyn Unknown>>) {
    for el in v {
        println!("{}", el.serialize())
    }
}
struct NodeGraph<T: PartialEq + Debug> {
    value: T,
    adjacents: Vec<Rc<NodeGraph<T>>>,
}
impl<T: PartialEq + Debug + Copy> NodeGraph<T> {
    fn new(node: T, graph: Vec<Rc<NodeGraph<T>>>) -> Self {
        Self {
            value: node,
            adjacents: graph,
        }
    }

    fn get_value(&self) -> &T {
        &self.value
    }

    fn dfs(&self, visited: &mut Vec<Rc<NodeGraph<T>>>) {
        for node in &self.adjacents {
            if visited.iter().find(|e| e.value == node.value).is_none() {
                visited.push(node.clone());
                node.dfs(visited);
            }
        }
    }
}

impl<T: PartialEq + Debug + Copy> Debug for NodeGraph<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut values = vec![];
        for node in &self.adjacents {
            values.push(node.value);
        }
        write!(f, "[value: {:?}, adjacents: \"{:?}\"]", self.value, values)
    }
}

struct Graph<T: PartialEq + Debug + Copy> {
    nodes: Vec<Rc<NodeGraph<T>>>,
}

impl<T: PartialEq + Debug + Copy> Graph<T> {
    fn new(nodes: Vec<Rc<NodeGraph<T>>>) -> Self {
        Self { nodes }
    }

    fn dfs(&self, root: Rc<NodeGraph<T>>) -> Vec<Rc<NodeGraph<T>>> {
        let root = self
            .nodes
            .iter()
            .find(|e| e.value == root.value)
            .unwrap();
        let mut visited = vec![root.clone()];
        root.dfs(&mut visited);
        visited
    }
}

mod tests {
    use std::rc::Rc;

    use crate::midterm2_mock::{getter_function, even_module, odd_module};

    use super::{BinIter, List, Task, SumTask, Tasker, LenTask, CloneAndDouble, get_vec, print_vec, NodeGraph, Graph};

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

    #[test]
    fn ex2_6() {
        let mut list: List<i32> = List::new();
        list.push(1);
        list.push(2);
        list.push(3);
        let mut list1: List<i32> = List::new();
        list1.push(1);
        list1.push(2);
        list1.push(3);
        debug_assert_eq!(list, list1);
        list1.push(4);
        debug_assert_ne!(list, list1);
        let mut list1: List<i32> = List::new();
        list1.push(1);
        list1.push(1);
        list1.push(3);
        debug_assert_ne!(list, list1);
    }

    #[test]
    fn ex3_1() {
        macro_rules! sum_task {
            (let $task: ident =$n1: literal + $n2: literal) => {
                let $task: Box<dyn Task> = Box::new(SumTask::new($n1, $n2));
            };
        }
    
        sum_task!(let t1 = 1+1);
        sum_task!(let t2 = 2+2);
        sum_task!(let t3 = 3+3);
        sum_task!(let t4 = 4+4);
        sum_task!(let t5 = 5+5);
        sum_task!(let t6 = 6+6);
        sum_task!(let t7 = 7+7);
    
        let mut tasker = Tasker::new();
        let executer = tasker.get_executer();
    
        println!("{:?}",executer.execute_task());
    
        tasker.schedule_task(t1);
        tasker.schedule_task(t2);
    
        println!("{:?}",executer.execute_task());
    
        tasker.schedule_task(t3);
        tasker.schedule_task(t4);
        tasker.schedule_task(t5);
        tasker.schedule_task(t6);
        tasker.schedule_task(t7);
    
        println!("{:?}",executer.execute_task());
        println!("{:?}",executer.execute_task());
        println!("{:?}",executer.execute_task());
        println!("{:?}",executer.execute_task());
        println!("{:?}",executer.execute_task());
        println!("{:?}",executer.execute_task());
        println!("{:?}",executer.execute_task());
    }

    #[test]
    fn ex3_2() {
        macro_rules! sum_task {
            (let $task: ident =$n1: literal + $n2: literal) => {
                let $task: Box<dyn Task> = Box::new(SumTask::new($n1, $n2));
            };
        }
        macro_rules! len_task {
            (let $task: ident =$s: literal) => {
                let $task: Box<dyn Task> = Box::new(LenTask::new($s.to_owned()));
            };
        }
    
    
        sum_task!(let t1 = 10+1);
        len_task!(let t2 = "four");
        let mut tasker1 = Tasker::new();
        let mut tasker2 = tasker1.get_tasker();
        let executer1 = tasker2.get_executer();
        let executer2 = tasker1.get_executer();
        tasker1.schedule_task(t1);
        tasker2.schedule_task(t2);
        println!("{:?}",executer1.execute_task());
        println!("{:?}",executer2.execute_task());
    }

    #[test]
    fn ex4_1() {
        println!("odd constant: {}",odd_module::CONSTANT);
        println!("even constant: {}",even_module::CONSTANT);
        print!("test function: {}", getter_function::get_constant(100));
    }

    #[test]
    fn ex4_2() {
        println!("odd constant: {}",odd_module::CONSTANT);
        println!("even constant: {}",even_module::CONSTANT);
        println!("test function: {}", getter_function::get_constant(105));
    }

    #[test]
    fn ex5() {
        println!("{}",1u8.clone_and_double());
        println!("{}",1i8.clone_and_double());
        println!("{}",2u16.clone_and_double());
        println!("{}",2i16.clone_and_double());
        println!("{}",3u32.clone_and_double());
        println!("{}",3i32.clone_and_double());
        println!("{}",4u64.clone_and_double());
        println!("{}",4i64.clone_and_double());
    }

    #[test]
    fn ex6_1() {
        let mut v = get_vec();
        v.push(Box::new("hiii!".to_string()));
        v.push(Box::new(-587));
        v.push(Box::new("xyz".to_string()));
        v.push(Box::new(vec![4, 5, 6]));
        print_vec(&v);
    }

    #[test]
    fn ex6_2() {
        let mut v = get_vec();
        v.push(Box::new("invalid data".to_string()));
        v.push(Box::new(58));
        v.push(Box::new(987));
        v.push(Box::new(vec![vec!["hi1", "hi2"], vec!["hi3"]]));
        print_vec(&v);
    }

    #[test]
    fn ex7() {
        let n1 = Rc::new(NodeGraph::new(1, vec![]));
        let n2 = Rc::new(NodeGraph::new(2, vec![n1.clone()]));
        let n3 = Rc::new(NodeGraph::new(3, vec![]));
        let n4 = Rc::new(NodeGraph::new(4, vec![n1.clone(), n3.clone()]));
        let n5 = Rc::new(NodeGraph::new(5, vec![n2.clone(), n4.clone()]));
        let n6 = Rc::new(NodeGraph::new(6, vec![n5.clone(), n4.clone()]));
        let n7 = Rc::new(NodeGraph::new(7, vec![n2.clone(), n4.clone()]));

        let graph = Graph::new(vec![
            n1.clone(),
            n2.clone(),
            n3.clone(),
            n4.clone(),
            n5.clone(),
            n6.clone(),
            n7.clone(),
        ]);

        let mut paths: Vec<Vec<Rc<NodeGraph<i32>>>> = vec![];
        for n in graph.nodes.iter() {
            paths.push(graph.dfs(n.clone()))
        }

        paths.iter().for_each(|path| {
            println!("{:?}", path);
        });
    }
}