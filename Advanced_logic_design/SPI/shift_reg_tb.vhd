library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity register_tb is
end entity;

architecture rtl of register_tb is
  component shift_reg is
    port
    (
      clk : in std_logic;
      reset : in std_logic;
      shift_enable : in std_logic;
      load_data : in std_logic_vector(7 downto 0);
      load_enable : in std_logic;
      data_out : out std_logic
    );
  end component;

  signal clk : std_logic := '0';
  signal reset : std_logic := '0';
  signal shift_enable : std_logic := '0';
  signal load_data : std_logic_vector(7 downto 0) := ( others => '0' );
  signal load_enable : std_logic := '0';
  signal data_out : std_logic := '0';
begin
  register_0 : shift_reg
  port map
  (
    clk => clk,
    reset => reset,
    shift_enable => shift_enable,
    load_data => load_data,
    load_enable => load_enable,
    data_out => data_out
  );

  clk <= not(clk) after 1 us;
  process begin
    reset <= '1';
    wait for 10 us;
    reset <= '0';
    load_data <= "10111010";
    load_enable <= '1';
    wait for 100 us;
    load_enable <= '0';
    shift_enable <= '1';
    wait for 200 us;
    reset <= '1';
    wait;
  end process;
end architecture;