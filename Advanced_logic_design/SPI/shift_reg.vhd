-- Library declarations
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- A shift counter
entity shift_reg is
  port
  (
    -- System clock
    clk : in std_logic;
    -- System reset (active high)
    reset : in std_logic;
    -- Shift enable
    shift_enable : in std_logic;
    -- Load enable
    load_enable : in std_logic;
    -- Parallel data input
    parallel_in : in std_logic_vector(7 downto 0);
    -- Data in shift register
    data : out std_logic_vector(7 downto 0);
    -- Serial data in
    data_in: in std_logic;
    -- Serial data out
    data_out: out std_logic
  );
end entity shift_reg;

architecture rtl of shift_reg is
begin
  process ( clk, reset, load_enable )
  begin
    if reset = '1' then
      data_out <= 'Z';
      data <= ( others => '0' );
    elsif rising_edge(clk) then
      if shift_enable = '1' then
        data <= data(data'high - 1 downto data'low) & data_in;
        data_out <= data(data'high);
      end if;
    elsif load_enable = '1' then
      data <= parallel_in;
    end if;
  end process;
end architecture;