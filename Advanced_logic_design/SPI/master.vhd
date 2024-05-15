-- Library declarations
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
-- A generic Master SPI interface
entity spi_master is
  generic
  (
    -- Number of bits to transfer
    NUM  : natural   := 8;
    CPOL : std_logic := '1';
    CPHA : std_logic := '1'
  );
  port
  (
    -- System clock
    clk : in std_logic;
    -- System reset (active low)
    reset : in std_logic;
    -- Command: load the data to be sent
    load_data : in std_logic;
    -- Data: the data to be sent
    data : in std_logic_vector(7 downto 0);
    -- Data currently in shift register
    shift_data : out std_logic_vector( 7 downto 0 );
    -- Command: start data transmission
    start : in std_logic;
    -- SPI interface signals (Master)
    MOSI : out std_logic;
    MISO : in std_logic;
    SCK  : out std_logic;
    SS   : out std_logic
  );
end entity spi_master;

architecture rtl of spi_master is
  component shift_reg is
    port
    (
      clk : in std_logic;
      reset : in std_logic;
      shift_enable : in std_logic;
      shift_data: out std_logic_vector(7 downto 0);
      load_data : in std_logic_vector(7 downto 0);
      load_enable : in std_logic;
      data_out : out std_logic
    );
  end component;
  component counter is
    generic (
        counter_size : integer := 3
    );
    port (
        clk : in std_logic;
        reset : in std_logic;
        count_enable : in std_logic;
        tc : out std_logic
    );
  end component;
  type t_State is (Sending, Receiving, Idle);
  signal state : t_State := Idle;
  signal shift_enable: std_logic := '0';
  signal shift_load: std_logic_vector(7 downto 0) := (others => '0');
  signal load_enable: std_logic := '0';
  signal data_out: std_logic := '0';
  signal bit_count_en: std_logic := '0';
  signal bit_count_tc: std_logic := '0';
  signal clk_count_en: std_logic := '0';
  signal clk_count_tc: std_logic := '0';
  signal data_in: std_logic_vector(7 downto 0);
begin
  output_register : shift_reg
  port map
  (
    clk => clk,
    reset => reset,
    shift_enable => shift_enable,
    shift_data => shift_data,
    load_data => shift_load,
    load_enable => load_enable,
    data_out => data_out
  );
  bit_counter: counter
  generic map (
    counter_size => 3
  )
  port map (
    clk => clk,
    reset => reset,
    count_enable => bit_count_en,
    tc => bit_count_tc
  );
  clk_counter: counter
  generic map (
    counter_size => 4
  )
  port map (
    clk => clk,
    reset => reset,
    count_enable => clk_count_en,
    tc => clk_count_tc
  );

  process ( clk, reset, clk_count_tc )
  begin
    if reset = '1' then
      MOSI <= '0';
      SCK <= '0';
      SS <= '0';
    else
      if rising_edge ( clk ) then
        if state = Idle then
          if load_data = '1' then
            shift_load <= data;
          elsif start = '1' then
            bit_count_en <= '1';
            shift_enable <= '1';
            state <= Sending;
          end if;
        end if;
      elsif rising_edge( clk_count_tc ) then
        case state is
          when Sending =>
            if bit_count_tc = '1' then
              shift_enable <= '0';
              bit_count_en <= '0';
              state <= Idle;
            else
              MOSI <= data_out;
            end if;
          when Receiving =>
            data_in <= data_in(data_in'high -1 downto data_in'low) & MISO;
            if bit_count_tc = '1' then
              bit_count_en <= '0';
              state <= Idle;
            end if;
          when Idle =>
        end case;
      end if;
    end if;
  end process;
end architecture;