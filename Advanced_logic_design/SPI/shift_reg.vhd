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
    -- Data to load
    load_data : in std_logic_vector(7 downto 0);
    -- Data in shift register
    shift_data : out std_logic_vector(7 downto 0);
    -- Load enable
    load_enable: in std_logic;
    -- Data out
    data_out: out std_logic
  );
end entity shift_reg;

architecture rtl of shift_reg is
begin
  process ( clk, reset )
  begin
    if reset = '1' then
      shift_data <= ( others => '0' );
      data_out <= '0';
    elsif rising_edge( clk ) then
      data_out <= '0';
      if load_enable = '1' then
        shift_data <= load_data;
      elsif shift_enable = '1' then
        shift_data <= shift_data(shift_data'high - 1 downto shift_data'low) & '0';
        data_out <= shift_data(shift_data'high);
      end if;
    end if;
  end process;
end architecture;