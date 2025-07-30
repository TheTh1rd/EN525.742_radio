vlib questa_lib/work
vlib questa_lib/msim

vlib questa_lib/msim/xbip_utils_v3_0_14
vlib questa_lib/msim/axi_utils_v2_0_10
vlib questa_lib/msim/mult_gen_v12_0_22
vlib questa_lib/msim/xbip_dsp48_wrapper_v3_0_6
vlib questa_lib/msim/xbip_pipe_v3_0_10
vlib questa_lib/msim/floating_point_v7_1_19
vlib questa_lib/msim/dds_compiler_v6_0_26
vlib questa_lib/msim/xil_defaultlib

vmap xbip_utils_v3_0_14 questa_lib/msim/xbip_utils_v3_0_14
vmap axi_utils_v2_0_10 questa_lib/msim/axi_utils_v2_0_10
vmap mult_gen_v12_0_22 questa_lib/msim/mult_gen_v12_0_22
vmap xbip_dsp48_wrapper_v3_0_6 questa_lib/msim/xbip_dsp48_wrapper_v3_0_6
vmap xbip_pipe_v3_0_10 questa_lib/msim/xbip_pipe_v3_0_10
vmap floating_point_v7_1_19 questa_lib/msim/floating_point_v7_1_19
vmap dds_compiler_v6_0_26 questa_lib/msim/dds_compiler_v6_0_26
vmap xil_defaultlib questa_lib/msim/xil_defaultlib

vcom -work xbip_utils_v3_0_14  -93  \
"../../../ipstatic/hdl/xbip_utils_v3_0_vh_rfs.vhd" \

vcom -work axi_utils_v2_0_10  -93  \
"../../../ipstatic/hdl/axi_utils_v2_0_vh_rfs.vhd" \

vcom -work mult_gen_v12_0_22  -93  \
"../../../ipstatic/hdl/mult_gen_v12_0_vh_rfs.vhd" \

vcom -work xbip_dsp48_wrapper_v3_0_6  -93  \
"../../../ipstatic/hdl/xbip_dsp48_wrapper_v3_0_vh_rfs.vhd" \

vcom -work xbip_pipe_v3_0_10  -93  \
"../../../ipstatic/hdl/xbip_pipe_v3_0_vh_rfs.vhd" \

vcom -work floating_point_v7_1_19  -93  \
"../../../ipstatic/hdl/floating_point_v7_1_rfs.vhd" \

vlog -work floating_point_v7_1_19  -incr -mfcu  \
"../../../ipstatic/hdl/floating_point_v7_1_rfs.v" \

vcom -work dds_compiler_v6_0_26  -2008  \
"../../../ipstatic/hdl/float_pkg.vhd" \

vcom -work dds_compiler_v6_0_26  -93  \
"../../../ipstatic/hdl/dds_compiler_v6_0_viv_comp.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0_comp.vhd" \
"../../../ipstatic/hdl/pkg_dds_compiler_v6_0.vhd" \
"../../../ipstatic/hdl/pkg_beta.vhd" \
"../../../ipstatic/hdl/pkg_alpha.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0_hdl_comps.vhd" \
"../../../ipstatic/hdl/dither_wrap.vhd" \
"../../../ipstatic/hdl/pipe_add.vhd" \
"../../../ipstatic/hdl/lut_ram.vhd" \
"../../../ipstatic/hdl/lut5_ram.vhd" \
"../../../ipstatic/hdl/flt_ufma_consts.vhd" \
"../../../ipstatic/hdl/flt_ufma.vhd" \
"../../../ipstatic/hdl/sin_cos.vhd" \

vcom -work dds_compiler_v6_0_26  -2008  \
"../../../ipstatic/hdl/sin_cos_fp.vhd" \

vcom -work dds_compiler_v6_0_26  -93  \
"../../../ipstatic/hdl/sin_cos_fp_reconstruct.vhd" \

vcom -work dds_compiler_v6_0_26  -2008  \
"../../../ipstatic/hdl/sin_cos_fp_partition.vhd" \

vcom -work dds_compiler_v6_0_26  -93  \
"../../../ipstatic/hdl/sin_cos_quad_rast.vhd" \
"../../../ipstatic/hdl/dsp48_wrap.vhd" \
"../../../ipstatic/hdl/accum.vhd" \
"../../../ipstatic/hdl/raster_accum.vhd" \
"../../../ipstatic/hdl/multadd.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0_eff_lut.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0_eff.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0_rdy.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0_core.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0_viv.vhd" \
"../../../ipstatic/hdl/dds_compiler_v6_0.vhd" \

vcom -work xil_defaultlib  -93  \
"../../../../../../../lab5.gen/sources_1/ip/dds_compiler_1/sim/dds_compiler_1.vhd" \

vlog -work xil_defaultlib \
"glbl.v"

