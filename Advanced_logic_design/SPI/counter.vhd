-- Library declarations
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- A generic counter
entity counter is
  generic
  (
    -- Size of counter
    counter_size: natural := 3
  );
  port
  (
    -- System clock
    clk : in std_logic;
    -- System reset (active high)
    reset : in std_logic;
    -- Count enable
    count_enable: in std_logic;
    -- Terminal count
    tc: out std_logic
  );
end entity counter;

architecture rtl of counter is
  signal count: unsigned (counter_size - 1 downto 0);
begin
  process ( clk, reset )
  begin
    if reset = '1' then
      count <= ( others => '1' );
    elsif rising_edge( clk ) then
      if count_enable = '1'then
        count <= count - 1;
        if count = 0 then
          tc <= '1';
          count <= ( others => '1' );
        else
          tc <= '0';
        end if;
      end if;
    end if;
  end process;
end architecture;