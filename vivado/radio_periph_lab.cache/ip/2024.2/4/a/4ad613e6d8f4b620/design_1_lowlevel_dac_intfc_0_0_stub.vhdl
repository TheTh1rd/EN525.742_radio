-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2024.2 (win64) Build 5239630 Fri Nov 08 22:35:27 MST 2024
-- Date        : Tue Jul 29 15:42:38 2025
-- Host        : main-pc running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ design_1_lowlevel_dac_intfc_0_0_stub.vhdl
-- Design      : design_1_lowlevel_dac_intfc_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  Port ( 
    resetn : in STD_LOGIC;
    clk125 : in STD_LOGIC;
    data_word : in STD_LOGIC_VECTOR ( 31 downto 0 );
    sdata : out STD_LOGIC;
    lrck : out STD_LOGIC;
    bclk : out STD_LOGIC;
    mclk : out STD_LOGIC;
    latched_data : out STD_LOGIC;
    valid : in STD_LOGIC
  );

  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "design_1_lowlevel_dac_intfc_0_0,lowlevel_dac_intfc,{}";
  attribute downgradeipidentifiedwarnings : string;
  attribute downgradeipidentifiedwarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "yes";
  attribute ip_definition_source : string;
  attribute ip_definition_source of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "package_project";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture stub of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  attribute syn_black_box : boolean;
  attribute black_box_pad_pin : string;
  attribute syn_black_box of stub : architecture is true;
  attribute black_box_pad_pin of stub : architecture is "resetn,clk125,data_word[31:0],sdata,lrck,bclk,mclk,latched_data,valid";
  attribute x_interface_info : string;
  attribute x_interface_info of resetn : signal is "xilinx.com:signal:reset:1.0 resetn RST";
  attribute x_interface_mode : string;
  attribute x_interface_mode of resetn : signal is "slave resetn";
  attribute x_interface_parameter : string;
  attribute x_interface_parameter of resetn : signal is "XIL_INTERFACENAME resetn, POLARITY ACTIVE_LOW, INSERT_VIP 0";
  attribute x_interface_info of clk125 : signal is "xilinx.com:signal:clock:1.0 clk125 CLK";
  attribute x_interface_mode of clk125 : signal is "slave clk125";
  attribute x_interface_parameter of clk125 : signal is "XIL_INTERFACENAME clk125, ASSOCIATED_RESET resetn, ASSOCIATED_BUSIF data_in, FREQ_TOLERANCE_HZ -1, FREQ_HZ 125000000, PHASE 0.0, CLK_DOMAIN design_1_clk125, INSERT_VIP 0";
  attribute x_interface_info of data_word : signal is "xilinx.com:interface:axis:1.0 data_in TDATA";
  attribute x_interface_mode of data_word : signal is "slave data_in";
  attribute x_interface_parameter of data_word : signal is "XIL_INTERFACENAME data_in, TDATA_NUM_BYTES 4, TDEST_WIDTH 0, TID_WIDTH 0, TUSER_WIDTH 0, HAS_TREADY 1, HAS_TSTRB 0, HAS_TKEEP 0, HAS_TLAST 0, FREQ_HZ 125000000, PHASE 0.0, CLK_DOMAIN design_1_clk125, LAYERED_METADATA undef, INSERT_VIP 0";
  attribute x_interface_info of latched_data : signal is "xilinx.com:interface:axis:1.0 data_in TREADY";
  attribute x_interface_info of valid : signal is "xilinx.com:interface:axis:1.0 data_in TVALID";
  attribute x_core_info : string;
  attribute x_core_info of stub : architecture is "lowlevel_dac_intfc,Vivado 2024.2";
begin
end;
