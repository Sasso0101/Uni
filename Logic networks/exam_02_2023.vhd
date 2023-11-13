library IEEE;
  use IEEE.std_logic_1164.all;
  use IEEE.numeric_std.all;

entity exam is
  port (
    l, clk, rst : in  std_logic;
    s, p        : out std_logic
  );
end entity;

architecture behavioral of exam is
  type state is (a, b, c, d);
  signal present_state, next_state : state;
begin
  seq: process (clk, rst)
  begin
    if rst = '0' then
      present_state <= a;
    elsif rising_edge(clk) then
      present_state <= next_state;
    end if;
  end process;

  future_state: process (present_state, l)
  begin
    case present_state is
      when a =>
        if l = '0' then
          next_state <= d;
        else
          next_state <= b;
        end if;
      when b =>
        if l = '0' then
          next_state <= c;
        else
          next_state <= a;
        end if;
      when c =>
        if l = '0' then
          next_state <= d;
        else
          next_state <= b;
        end if;
      when d =>
        if l = '0' then
          next_state <= d;
        else
          next_state <= b;
        end if;
    end case;
  end process;

  output: process (clk, rst)
  begin
    if rst = '0' then
      s <= 0;
      p <= 1;
    elsif rising_edge(clk) then
      case next_state is
        when a =>
          s <= 0;
          p <= 1;
        when b =>
          s <= 0;
          p <= 0;
        when c =>
          s <= 1;
          p <= 1;
        when d =>
          s <= 1;
          p <= 0;
      end case;
    end if;
  end process;
end architecture;
