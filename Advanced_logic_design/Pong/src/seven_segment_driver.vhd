library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.utilities.all;

entity seven_segment_driver is
    generic (
        size : integer := 19
    );
    port (
        clk : in std_logic;
        res : in std_logic;
        digit1, digit2 : in std_logic_vector( 6 downto 0 );
        CA, CB, CC, CD, CE, CF, CG, DP : out std_logic;
        AN : out std_logic_vector( 7 downto 0 )
    );
end seven_segment_driver;

architecture Behavioral of seven_segment_driver is
  -- We will use a counter to derive the frequency for the displays
  -- Clock is 100 MHz, we use 3 bits to address the display, so we count every
  -- size - 3 bits. To get ~100 Hz per digit, we need 20 bits, so that we divide
  -- by 2^20.
  signal flick_counter : unsigned( size - 1 downto 0 );
  -- The digit is temporarily stored here
  signal digit : std_logic_vector( 3 downto 0 );
  -- Collect the values of the cathodes here
  signal cathodes : std_logic_vector( 7 downto 0 );
  
  signal bin1, bin2 : std_logic_vector(7 downto 0);

    component blk_mem_gen_1
        port (
            clka : IN STD_LOGIC;
            addra : IN STD_LOGIC_VECTOR(6 DOWNTO 0);
            douta : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
            clkb : IN STD_LOGIC;
            addrb : IN STD_LOGIC_VECTOR(6 DOWNTO 0);
            doutb : OUT STD_LOGIC_VECTOR(7 DOWNTO 0) 
        );
    end component;
begin
    -- ROM used as LUT to convert from binary to decimal (0 to 99)
    bcd_lut : blk_mem_gen_1
    PORT MAP (
        clka => clk,
        addra => digit1,
        douta => bin1,
        clkb => clk,
        addrb => digit2,
        doutb => bin2
    );

  -- Divide the clock
    process ( clk, res ) begin
        if res = '0' then
            flick_counter <= ( others => '0' );
        elsif rising_edge( clk ) then
            flick_counter <= flick_counter + 1;
        end if;
    end process;

  -- Select the anode
    with flick_counter( size - 1 downto size - 3 ) select AN <=
        "11111110" when "000",
        "11111101" when "001",
        "11111011" when "010",
        "11110111" when "011",
        "11101111" when "100",
        "11011111" when "101",
        "10111111" when "110",
        "01111111" when "111",
        "11111111"  when others;

  -- Select the digit
    with flick_counter( size - 1 downto size - 3 ) select digit <=
        bin2(3 downto 0) when "000",
        bin2(7 downto 4) when "001",
        (others => '0')  when "010",
        (others => '0') when "011",
        bin1(3 downto 0) when "100",
        bin1(7 downto 4) when "101",
        (others => '0') when "110",
        (others => '0') when "111",
        (others => '0') when others;

  -- Decode the digit
    with digit select cathodes <=
        -- DP, CG, CF, CE, CD, CC, CB, CA
        "11000000" when "0000",
        "11111001" when "0001",
        "10100100" when "0010",
        "10110000" when "0011",
        "10011001" when "0100",
        "10010010" when "0101",
        "10000010" when "0110",
        "11111000" when "0111",
        "10000000" when "1000",
        "10010000" when "1001",
        "10001000" when "1010",
        "10000011" when "1011",
        "11000110" when "1100",
        "10100001" when "1101",
        "10000110" when "1110",
        "10001110" when others;

    DP <= cathodes(7);
    CG <= cathodes(6);
    CF <= cathodes(5);
    CE <= cathodes(4);
    CD <= cathodes(3);
    CC <= cathodes(2);
    CB <= cathodes(1);
    CA <= cathodes(0);

end behavioral;
