package model;

import data.AddOnIngredient;
import data.Ingredient;
import data.Pizza;
import data.VeganPizza;

import java.io.*;
import java.util.ArrayList;
import java.util.LinkedList;

public class Menu {
    LinkedList<Pizza> menu = new LinkedList<>();
    ArrayList<Ingredient> ingredients = new ArrayList<>();
    ArrayList<AddOnIngredient> addOns = new ArrayList<>();

    public void importMenu(String filename) {
        String path = new File("src/files/"+filename).getAbsolutePath();
        try (
                FileReader f = new FileReader(path);
                BufferedReader reader = new BufferedReader(f)
                ) {
            String line = "";
            while ((line = reader.readLine()) != null) {
                if (line.length() == 0) continue;
                String[] values = line.split(",");
                for (int i = 0; i < values.length; i++) {
                    values[i] = values[i].strip();
                }
                Pizza p;
                if (values[2].equals("0")) p = new Pizza(values[0], Integer.parseInt(values[1]));
                else p = new VeganPizza(values[0], Integer.parseInt(values[1]));
                for (int i = 3; i < values.length; i++) {
                    int index = ingredients.indexOf(new Ingredient(values[i]));
                    if (index == -1) p.addIngredient(new Ingredient(values[i]));
                    else p.addIngredient(ingredients.get(index));
                }
                menu.add(p);
            }
        } catch (FileNotFoundException e) {
            System.out.println("File was not found!");
        } catch (IOException e) {
            System.out.println("There was an exception during the reading of the file!");
        }
    }

    public void importAddOns(String filename) {
        String path = new File("src/files/"+filename).getAbsolutePath();
        try (
                FileReader f = new FileReader(path);
                BufferedReader reader = new BufferedReader(f)
        ) {
            String line = "";
            while ((line = reader.readLine()) != null) {
                if (line.length() == 0) continue;
                String[] values = line.split(",");
                for (int i = 0; i < values.length; i++) {
                    values[i] = values[i].strip();
                }
                addOns.add(new AddOnIngredient(values[0], Integer.parseInt(values[1])));
            }
        } catch (FileNotFoundException e) {
            System.out.println("File was not found!");
        } catch (IOException e) {
            System.out.println("There was an exception during the reading of the file!");
        }
    }

    public String toString(LinkedList<Pizza> menu) {
        String out = "";
        for (Pizza p: menu) {
            out += p.toString() + "\n";
        }
        return out;
    }

    public String toString() {
        return toString(menu);
    }

    public LinkedList<Pizza> getMenu() {
        return menu;
    }

    public LinkedList<Pizza> getVegan() {
        LinkedList<Pizza> veganPizzas = new LinkedList<>();
        for (Pizza p: menu) {
            if (p instanceof VeganPizza) veganPizzas.add(p);
        }
        return veganPizzas;
    }

    public LinkedList<Pizza> getNonVegan() {
        LinkedList<Pizza> nonVeganPizzas = new LinkedList<>();
        for (Pizza p: menu) {
            if (!(p instanceof VeganPizza)) nonVeganPizzas.add(p);
        }
        return nonVeganPizzas;
    }

    public ArrayList<AddOnIngredient> getAddOns() {
        return addOns;
    }

    public String addOnsToString() {
        String out = "";
        for (AddOnIngredient i: addOns) {
            out += i.toString() + "\n";
        }
        return out;
    }

    public Pizza getPizza(String name) {
        for (Pizza p: menu) {
            if (p.getName().equals(name)) return p;
        }
        return null;
    }

    public static void main(String[] args) {
        Menu m = new Menu();
        m.importMenu("menu.txt");
        m.importAddOns("addons.txt");
        System.out.println(m);
    }
}
