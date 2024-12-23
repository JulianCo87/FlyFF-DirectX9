#include "stdafx.h"
#include "NpcScript.h"

struct TABLES
{
	char* name;
	void ( CNpcScript::*pmf)();
};

TABLES g_tables[] = {
	{"dudk_drian_0", &CNpcScript::dudk_drian_0},
	{"dudk_drian_1", &CNpcScript::dudk_drian_1},
	{"dudk_drian_2", &CNpcScript::dudk_drian_2},
	{"dudk_drian_3", &CNpcScript::dudk_drian_3},
	{"dudk_drian_4", &CNpcScript::dudk_drian_4},
	{"dudk_drian_5", &CNpcScript::dudk_drian_5},
	{"dudk_drian_6", &CNpcScript::dudk_drian_6},
	{"dudk_drian_7", &CNpcScript::dudk_drian_7},
	{"dudk_drian_8", &CNpcScript::dudk_drian_8},
	{"dudk_drian_9", &CNpcScript::dudk_drian_9},
	{"dudk_drian_10", &CNpcScript::dudk_drian_10},
	{"dudk_drian_11", &CNpcScript::dudk_drian_11},
	{"dudk_kazen_0", &CNpcScript::dudk_kazen_0},
	{"dudk_kazen_1", &CNpcScript::dudk_kazen_1},
	{"dudk_kazen_2", &CNpcScript::dudk_kazen_2},
	{"dudk_kazen_3", &CNpcScript::dudk_kazen_3},
	{"dudk_kazen_4", &CNpcScript::dudk_kazen_4},
	{"dudk_kazen_5", &CNpcScript::dudk_kazen_5},
	{"dudk_kazen_6", &CNpcScript::dudk_kazen_6},
	{"dudk_kazen_7", &CNpcScript::dudk_kazen_7},
	{"dudk_kazen_8", &CNpcScript::dudk_kazen_8},
	{"dudk_kazen_9", &CNpcScript::dudk_kazen_9},
	{"dudk_kazen_10", &CNpcScript::dudk_kazen_10},
	{"dudk_nevil_0", &CNpcScript::dudk_nevil_0},
	{"dudk_nevil_1", &CNpcScript::dudk_nevil_1},
	{"dudk_nevil_2", &CNpcScript::dudk_nevil_2},
	{"dudk_nevil_3", &CNpcScript::dudk_nevil_3},
	{"dudk_nevil_4", &CNpcScript::dudk_nevil_4},
	{"dudk_nevil_5", &CNpcScript::dudk_nevil_5},
	{"dudk_nevil_6", &CNpcScript::dudk_nevil_6},
	{"dudk_nevil_7", &CNpcScript::dudk_nevil_7},
	{"dudk_nevil_8", &CNpcScript::dudk_nevil_8},
	{"dudk_nevil_12", &CNpcScript::dudk_nevil_12},
	{"dudk_nevil_10", &CNpcScript::dudk_nevil_10},
	{"kepe_ciel_0", &CNpcScript::kepe_ciel_0},
	{"kepe_ciel_1", &CNpcScript::kepe_ciel_1},
	{"kepe_ciel_9", &CNpcScript::kepe_ciel_9},
	{"kepe_ciel_10", &CNpcScript::kepe_ciel_10},
	{"kepe_heron_0", &CNpcScript::kepe_heron_0},
	{"kepe_heron_1", &CNpcScript::kepe_heron_1},
	{"kepe_heron_2", &CNpcScript::kepe_heron_2},
	{"kepe_heron_3", &CNpcScript::kepe_heron_3},
	{"kepe_heron_4", &CNpcScript::kepe_heron_4},
	{"kepe_heron_5", &CNpcScript::kepe_heron_5},
	{"kepe_heron_6", &CNpcScript::kepe_heron_6},
	{"kepe_heron_7", &CNpcScript::kepe_heron_7},
	{"kepe_heron_8", &CNpcScript::kepe_heron_8},
	{"kepe_heron_9", &CNpcScript::kepe_heron_9},
	{"kepe_heron_13", &CNpcScript::kepe_heron_13},
	{"kepe_heron_10", &CNpcScript::kepe_heron_10},
	{"kepe_rocbin_0", &CNpcScript::kepe_rocbin_0},
	{"kepe_rocbin_1", &CNpcScript::kepe_rocbin_1},
	{"kepe_rocbin_9", &CNpcScript::kepe_rocbin_9},
	{"kepe_rocbin_10", &CNpcScript::kepe_rocbin_10},
	{"kepe_shun_0", &CNpcScript::kepe_shun_0},
	{"kepe_shun_1", &CNpcScript::kepe_shun_1},
	{"kepe_shun_9", &CNpcScript::kepe_shun_9},
	{"kepe_shun_10", &CNpcScript::kepe_shun_10},
	{"kepe_yuna_0", &CNpcScript::kepe_yuna_0},
	{"kepe_yuna_1", &CNpcScript::kepe_yuna_1},
	{"kepe_yuna_9", &CNpcScript::kepe_yuna_9},
	{"kepe_yuna_10", &CNpcScript::kepe_yuna_10},
	{"mada_achaben_0", &CNpcScript::mada_achaben_0},
	{"mada_achaben_1", &CNpcScript::mada_achaben_1},
	{"mada_achaben_2", &CNpcScript::mada_achaben_2},
	{"mada_achaben_3", &CNpcScript::mada_achaben_3},
	{"mada_achaben_4", &CNpcScript::mada_achaben_4},
	{"mada_achaben_5", &CNpcScript::mada_achaben_5},
	{"mada_achaben_6", &CNpcScript::mada_achaben_6},
	{"mada_achaben_8", &CNpcScript::mada_achaben_8},
	{"mada_achaben_9", &CNpcScript::mada_achaben_9},
	{"mada_achaben_10", &CNpcScript::mada_achaben_10},
	{"mada_almani_0", &CNpcScript::mada_almani_0},
	{"mada_almani_1", &CNpcScript::mada_almani_1},
	{"mada_almani_2", &CNpcScript::mada_almani_2},
	{"mada_almani_3", &CNpcScript::mada_almani_3},
	{"mada_almani_4", &CNpcScript::mada_almani_4},
	{"mada_almani_5", &CNpcScript::mada_almani_5},
	{"mada_almani_6", &CNpcScript::mada_almani_6},
	{"mada_almani_8", &CNpcScript::mada_almani_8},
	{"mada_almani_9", &CNpcScript::mada_almani_9},
	{"mada_almani_14", &CNpcScript::mada_almani_14},
	{"mada_almani_15", &CNpcScript::mada_almani_15},
	{"mada_almani_10", &CNpcScript::mada_almani_10},
	{"mada_amadolka_0", &CNpcScript::mada_amadolka_0},
	{"mada_amadolka_1", &CNpcScript::mada_amadolka_1},
	{"mada_amadolka_2", &CNpcScript::mada_amadolka_2},
	{"mada_amadolka_3", &CNpcScript::mada_amadolka_3},
	{"mada_amadolka_4", &CNpcScript::mada_amadolka_4},
	{"mada_amadolka_5", &CNpcScript::mada_amadolka_5},
	{"mada_amadolka_6", &CNpcScript::mada_amadolka_6},
	{"mada_amadolka_8", &CNpcScript::mada_amadolka_8},
	{"mada_amadolka_9", &CNpcScript::mada_amadolka_9},
	{"mada_amadolka_10", &CNpcScript::mada_amadolka_10},
	{"mada_andre_0", &CNpcScript::mada_andre_0},
	{"mada_andre_1", &CNpcScript::mada_andre_1},
	{"mada_andre_2", &CNpcScript::mada_andre_2},
	{"mada_andre_3", &CNpcScript::mada_andre_3},
	{"mada_andre_4", &CNpcScript::mada_andre_4},
	{"mada_andre_5", &CNpcScript::mada_andre_5},
	{"mada_andre_6", &CNpcScript::mada_andre_6},
	{"mada_andre_7", &CNpcScript::mada_andre_7},
	{"mada_andre_8", &CNpcScript::mada_andre_8},
	{"mada_andre_12", &CNpcScript::mada_andre_12},
	{"mada_andre_10", &CNpcScript::mada_andre_10},
	{"mada_bernard_0", &CNpcScript::mada_bernard_0},
	{"mada_bernard_1", &CNpcScript::mada_bernard_1},
	{"mada_bernard_2", &CNpcScript::mada_bernard_2},
	{"mada_bernard_3", &CNpcScript::mada_bernard_3},
	{"mada_bernard_4", &CNpcScript::mada_bernard_4},
	{"mada_bernard_5", &CNpcScript::mada_bernard_5},
	{"mada_bernard_6", &CNpcScript::mada_bernard_6},
	{"mada_bernard_7", &CNpcScript::mada_bernard_7},
	{"mada_bernard_8", &CNpcScript::mada_bernard_8},
	{"mada_bernard_12", &CNpcScript::mada_bernard_12},
	{"mada_bernard_10", &CNpcScript::mada_bernard_10},
	{"mada_bernard_16", &CNpcScript::mada_bernard_16},
	{"mada_bolpor_0", &CNpcScript::mada_bolpor_0},
	{"mada_bolpor_1", &CNpcScript::mada_bolpor_1},
	{"mada_bolpor_2", &CNpcScript::mada_bolpor_2},
	{"mada_bolpor_3", &CNpcScript::mada_bolpor_3},
	{"mada_bolpor_4", &CNpcScript::mada_bolpor_4},
	{"mada_bolpor_5", &CNpcScript::mada_bolpor_5},
	{"mada_bolpor_6", &CNpcScript::mada_bolpor_6},
	{"mada_bolpor_8", &CNpcScript::mada_bolpor_8},
	{"mada_bolpor_9", &CNpcScript::mada_bolpor_9},
	{"mada_bolpor_10", &CNpcScript::mada_bolpor_10},
	{"mada_boneper_0", &CNpcScript::mada_boneper_0},
	{"mada_boneper_1", &CNpcScript::mada_boneper_1},
	{"mada_boneper_2", &CNpcScript::mada_boneper_2},
	{"mada_boneper_3", &CNpcScript::mada_boneper_3},
	{"mada_boneper_4", &CNpcScript::mada_boneper_4},
	{"mada_boneper_5", &CNpcScript::mada_boneper_5},
	{"mada_boneper_6", &CNpcScript::mada_boneper_6},
	{"mada_boneper_8", &CNpcScript::mada_boneper_8},
	{"mada_boneper_9", &CNpcScript::mada_boneper_9},
	{"mada_boneper_10", &CNpcScript::mada_boneper_10},
	{"mada_capafe_0", &CNpcScript::mada_capafe_0},
	{"mada_capafe_1", &CNpcScript::mada_capafe_1},
	{"mada_capafe_2", &CNpcScript::mada_capafe_2},
	{"mada_capafe_3", &CNpcScript::mada_capafe_3},
	{"mada_capafe_4", &CNpcScript::mada_capafe_4},
	{"mada_capafe_5", &CNpcScript::mada_capafe_5},
	{"mada_capafe_6", &CNpcScript::mada_capafe_6},
	{"mada_capafe_8", &CNpcScript::mada_capafe_8},
	{"mada_capafe_9", &CNpcScript::mada_capafe_9},
	{"mada_capafe_10", &CNpcScript::mada_capafe_10},
	{"mada_cell_0", &CNpcScript::mada_cell_0},
	{"mada_cell_1", &CNpcScript::mada_cell_1},
	{"mada_cell_2", &CNpcScript::mada_cell_2},
	{"mada_cell_3", &CNpcScript::mada_cell_3},
	{"mada_cell_4", &CNpcScript::mada_cell_4},
	{"mada_cell_5", &CNpcScript::mada_cell_5},
	{"mada_cell_6", &CNpcScript::mada_cell_6},
	{"mada_cell_7", &CNpcScript::mada_cell_7},
	{"mada_cell_8", &CNpcScript::mada_cell_8},
	{"mada_cell_12", &CNpcScript::mada_cell_12},
	{"mada_cell_10", &CNpcScript::mada_cell_10},
	{"mada_cell_17", &CNpcScript::mada_cell_17},
	{"mada_chenbing_0", &CNpcScript::mada_chenbing_0},
	{"mada_chenbing_1", &CNpcScript::mada_chenbing_1},
	{"mada_chenbing_2", &CNpcScript::mada_chenbing_2},
	{"mada_chenbing_3", &CNpcScript::mada_chenbing_3},
	{"mada_chenbing_4", &CNpcScript::mada_chenbing_4},
	{"mada_chenbing_5", &CNpcScript::mada_chenbing_5},
	{"mada_chenbing_6", &CNpcScript::mada_chenbing_6},
	{"mada_chenbing_8", &CNpcScript::mada_chenbing_8},
	{"mada_chenbing_9", &CNpcScript::mada_chenbing_9},
	{"mada_chenbing_10", &CNpcScript::mada_chenbing_10},
	{"mada_cloneeshylop_0", &CNpcScript::mada_cloneeshylop_0},
	{"mada_cloneeshylop_1", &CNpcScript::mada_cloneeshylop_1},
	{"mada_cloneeshylop_2", &CNpcScript::mada_cloneeshylop_2},
	{"mada_cloneeshylop_3", &CNpcScript::mada_cloneeshylop_3},
	{"mada_cloneeshylop_4", &CNpcScript::mada_cloneeshylop_4},
	{"mada_cloneeshylop_5", &CNpcScript::mada_cloneeshylop_5},
	{"mada_cloneeshylop_6", &CNpcScript::mada_cloneeshylop_6},
	{"mada_cloneeshylop_8", &CNpcScript::mada_cloneeshylop_8},
	{"mada_cloneeshylop_9", &CNpcScript::mada_cloneeshylop_9},
	{"mada_cloneeshylop_18", &CNpcScript::mada_cloneeshylop_18},
	{"mada_cloneeshylop_10", &CNpcScript::mada_cloneeshylop_10},
	{"mada_colar_0", &CNpcScript::mada_colar_0},
	{"mada_colar_1", &CNpcScript::mada_colar_1},
	{"mada_colar_2", &CNpcScript::mada_colar_2},
	{"mada_colar_3", &CNpcScript::mada_colar_3},
	{"mada_colar_4", &CNpcScript::mada_colar_4},
	{"mada_colar_5", &CNpcScript::mada_colar_5},
	{"mada_colar_6", &CNpcScript::mada_colar_6},
	{"mada_colar_7", &CNpcScript::mada_colar_7},
	{"mada_colar_8", &CNpcScript::mada_colar_8},
	{"mada_colar_9", &CNpcScript::mada_colar_9},
	{"mada_colar_10", &CNpcScript::mada_colar_10},
	{"mada_condram_0", &CNpcScript::mada_condram_0},
	{"mada_condram_1", &CNpcScript::mada_condram_1},
	{"mada_condram_2", &CNpcScript::mada_condram_2},
	{"mada_condram_3", &CNpcScript::mada_condram_3},
	{"mada_condram_4", &CNpcScript::mada_condram_4},
	{"mada_condram_5", &CNpcScript::mada_condram_5},
	{"mada_condram_6", &CNpcScript::mada_condram_6},
	{"mada_condram_8", &CNpcScript::mada_condram_8},
	{"mada_condram_9", &CNpcScript::mada_condram_9},
	{"mada_condram_10", &CNpcScript::mada_condram_10},
	{"mada_corel_0", &CNpcScript::mada_corel_0},
	{"mada_corel_1", &CNpcScript::mada_corel_1},
	{"mada_corel_2", &CNpcScript::mada_corel_2},
	{"mada_corel_3", &CNpcScript::mada_corel_3},
	{"mada_corel_4", &CNpcScript::mada_corel_4},
	{"mada_corel_5", &CNpcScript::mada_corel_5},
	{"mada_corel_6", &CNpcScript::mada_corel_6},
	{"mada_corel_8", &CNpcScript::mada_corel_8},
	{"mada_corel_9", &CNpcScript::mada_corel_9},
	{"mada_corel_10", &CNpcScript::mada_corel_10},
	{"mada_cylor_0", &CNpcScript::mada_cylor_0},
	{"mada_cylor_1", &CNpcScript::mada_cylor_1},
	{"mada_cylor_2", &CNpcScript::mada_cylor_2},
	{"mada_cylor_3", &CNpcScript::mada_cylor_3},
	{"mada_cylor_4", &CNpcScript::mada_cylor_4},
	{"mada_cylor_5", &CNpcScript::mada_cylor_5},
	{"mada_cylor_6", &CNpcScript::mada_cylor_6},
	{"mada_cylor_8", &CNpcScript::mada_cylor_8},
	{"mada_cylor_9", &CNpcScript::mada_cylor_9},
	{"mada_cylor_10", &CNpcScript::mada_cylor_10},
	{"mada_drest_0", &CNpcScript::mada_drest_0},
	{"mada_drest_1", &CNpcScript::mada_drest_1},
	{"mada_drest_2", &CNpcScript::mada_drest_2},
	{"mada_drest_3", &CNpcScript::mada_drest_3},
	{"mada_drest_4", &CNpcScript::mada_drest_4},
	{"mada_drest_5", &CNpcScript::mada_drest_5},
	{"mada_drest_6", &CNpcScript::mada_drest_6},
	{"mada_drest_8", &CNpcScript::mada_drest_8},
	{"mada_drest_9", &CNpcScript::mada_drest_9},
	{"mada_drest_10", &CNpcScript::mada_drest_10},
	{"mada_eliff_0", &CNpcScript::mada_eliff_0},
	{"mada_eliff_1", &CNpcScript::mada_eliff_1},
	{"mada_eliff_2", &CNpcScript::mada_eliff_2},
	{"mada_eliff_3", &CNpcScript::mada_eliff_3},
	{"mada_eliff_4", &CNpcScript::mada_eliff_4},
	{"mada_eliff_5", &CNpcScript::mada_eliff_5},
	{"mada_eliff_6", &CNpcScript::mada_eliff_6},
	{"mada_eliff_7", &CNpcScript::mada_eliff_7},
	{"mada_eliff_8", &CNpcScript::mada_eliff_8},
	{"mada_eliff_9", &CNpcScript::mada_eliff_9},
	{"mada_eliff_10", &CNpcScript::mada_eliff_10},
	{"mada_ellend_0", &CNpcScript::mada_ellend_0},
	{"mada_ellend_1", &CNpcScript::mada_ellend_1},
	{"mada_ellend_2", &CNpcScript::mada_ellend_2},
	{"mada_ellend_3", &CNpcScript::mada_ellend_3},
	{"mada_ellend_4", &CNpcScript::mada_ellend_4},
	{"mada_ellend_5", &CNpcScript::mada_ellend_5},
	{"mada_ellend_6", &CNpcScript::mada_ellend_6},
	{"mada_ellend_8", &CNpcScript::mada_ellend_8},
	{"mada_ellend_9", &CNpcScript::mada_ellend_9},
	{"mada_ellend_10", &CNpcScript::mada_ellend_10},
	{"mada_eshylop_0", &CNpcScript::mada_eshylop_0},
	{"mada_eshylop_1", &CNpcScript::mada_eshylop_1},
	{"mada_eshylop_2", &CNpcScript::mada_eshylop_2},
	{"mada_eshylop_3", &CNpcScript::mada_eshylop_3},
	{"mada_eshylop_4", &CNpcScript::mada_eshylop_4},
	{"mada_eshylop_5", &CNpcScript::mada_eshylop_5},
	{"mada_eshylop_6", &CNpcScript::mada_eshylop_6},
	{"mada_eshylop_8", &CNpcScript::mada_eshylop_8},
	{"mada_eshylop_9", &CNpcScript::mada_eshylop_9},
	{"mada_eshylop_19", &CNpcScript::mada_eshylop_19},
	{"mada_eshylop_20", &CNpcScript::mada_eshylop_20},
	{"mada_eshylop_21", &CNpcScript::mada_eshylop_21},
	{"mada_eshylop_22", &CNpcScript::mada_eshylop_22},
	{"mada_eshylop_23", &CNpcScript::mada_eshylop_23},
	{"mada_eshylop_10", &CNpcScript::mada_eshylop_10},
	{"mada_fera_0", &CNpcScript::mada_fera_0},
	{"mada_fera_1", &CNpcScript::mada_fera_1},
	{"mada_fera_2", &CNpcScript::mada_fera_2},
	{"mada_fera_3", &CNpcScript::mada_fera_3},
	{"mada_fera_4", &CNpcScript::mada_fera_4},
	{"mada_fera_5", &CNpcScript::mada_fera_5},
	{"mada_fera_6", &CNpcScript::mada_fera_6},
	{"mada_fera_8", &CNpcScript::mada_fera_8},
	{"mada_fera_9", &CNpcScript::mada_fera_9},
	{"mada_fera_10", &CNpcScript::mada_fera_10},
	{"mada_furan_0", &CNpcScript::mada_furan_0},
	{"mada_furan_1", &CNpcScript::mada_furan_1},
	{"mada_furan_2", &CNpcScript::mada_furan_2},
	{"mada_furan_3", &CNpcScript::mada_furan_3},
	{"mada_furan_4", &CNpcScript::mada_furan_4},
	{"mada_furan_5", &CNpcScript::mada_furan_5},
	{"mada_furan_6", &CNpcScript::mada_furan_6},
	{"mada_furan_7", &CNpcScript::mada_furan_7},
	{"mada_furan_8", &CNpcScript::mada_furan_8},
	{"mada_furan_9", &CNpcScript::mada_furan_9},
	{"mada_furan_10", &CNpcScript::mada_furan_10},
	{"mada_guildcombatshop_0", &CNpcScript::mada_guildcombatshop_0},
	{"mada_guildcombatshop_1", &CNpcScript::mada_guildcombatshop_1},
	{"mada_guildcombatshop_2", &CNpcScript::mada_guildcombatshop_2},
	{"mada_guildcombatshop_3", &CNpcScript::mada_guildcombatshop_3},
	{"mada_guildcombatshop_4", &CNpcScript::mada_guildcombatshop_4},
	{"mada_guildcombatshop_5", &CNpcScript::mada_guildcombatshop_5},
	{"mada_guildcombatshop_6", &CNpcScript::mada_guildcombatshop_6},
	{"mada_guildcombatshop_7", &CNpcScript::mada_guildcombatshop_7},
	{"mada_guildcombatshop_8", &CNpcScript::mada_guildcombatshop_8},
	{"mada_guildcombatshop_9", &CNpcScript::mada_guildcombatshop_9},
	{"mada_guildcombatshop_10", &CNpcScript::mada_guildcombatshop_10},
	{"mada_hachal_0", &CNpcScript::mada_hachal_0},
	{"mada_hachal_1", &CNpcScript::mada_hachal_1},
	{"mada_hachal_2", &CNpcScript::mada_hachal_2},
	{"mada_hachal_3", &CNpcScript::mada_hachal_3},
	{"mada_hachal_4", &CNpcScript::mada_hachal_4},
	{"mada_hachal_5", &CNpcScript::mada_hachal_5},
	{"mada_hachal_6", &CNpcScript::mada_hachal_6},
	{"mada_hachal_8", &CNpcScript::mada_hachal_8},
	{"mada_hachal_9", &CNpcScript::mada_hachal_9},
	{"mada_hachal_10", &CNpcScript::mada_hachal_10},
	{"mada_haven_0", &CNpcScript::mada_haven_0},
	{"mada_haven_1", &CNpcScript::mada_haven_1},
	{"mada_haven_2", &CNpcScript::mada_haven_2},
	{"mada_haven_3", &CNpcScript::mada_haven_3},
	{"mada_haven_4", &CNpcScript::mada_haven_4},
	{"mada_haven_5", &CNpcScript::mada_haven_5},
	{"mada_haven_6", &CNpcScript::mada_haven_6},
	{"mada_haven_8", &CNpcScript::mada_haven_8},
	{"mada_haven_9", &CNpcScript::mada_haven_9},
	{"mada_haven_10", &CNpcScript::mada_haven_10},
	{"mada_heedan_0", &CNpcScript::mada_heedan_0},
	{"mada_heedan_1", &CNpcScript::mada_heedan_1},
	{"mada_heedan_2", &CNpcScript::mada_heedan_2},
	{"mada_heedan_3", &CNpcScript::mada_heedan_3},
	{"mada_heedan_4", &CNpcScript::mada_heedan_4},
	{"mada_heedan_5", &CNpcScript::mada_heedan_5},
	{"mada_heedan_6", &CNpcScript::mada_heedan_6},
	{"mada_heedan_8", &CNpcScript::mada_heedan_8},
	{"mada_heedan_9", &CNpcScript::mada_heedan_9},
	{"mada_heedan_10", &CNpcScript::mada_heedan_10},
	{"mada_heingard_0", &CNpcScript::mada_heingard_0},
	{"mada_heingard_1", &CNpcScript::mada_heingard_1},
	{"mada_heingard_2", &CNpcScript::mada_heingard_2},
	{"mada_heingard_3", &CNpcScript::mada_heingard_3},
	{"mada_heingard_4", &CNpcScript::mada_heingard_4},
	{"mada_heingard_5", &CNpcScript::mada_heingard_5},
	{"mada_heingard_6", &CNpcScript::mada_heingard_6},
	{"mada_heingard_7", &CNpcScript::mada_heingard_7},
	{"mada_heingard_8", &CNpcScript::mada_heingard_8},
	{"mada_heingard_9", &CNpcScript::mada_heingard_9},
	{"mada_heingard_10", &CNpcScript::mada_heingard_10},
	{"mada_hent_0", &CNpcScript::mada_hent_0},
	{"mada_hent_1", &CNpcScript::mada_hent_1},
	{"mada_hent_2", &CNpcScript::mada_hent_2},
	{"mada_hent_3", &CNpcScript::mada_hent_3},
	{"mada_hent_4", &CNpcScript::mada_hent_4},
	{"mada_hent_5", &CNpcScript::mada_hent_5},
	{"mada_hent_6", &CNpcScript::mada_hent_6},
	{"mada_hent_7", &CNpcScript::mada_hent_7},
	{"mada_hent_8", &CNpcScript::mada_hent_8},
	{"mada_hent_9", &CNpcScript::mada_hent_9},
	{"mada_hent_10", &CNpcScript::mada_hent_10},
	{"mada_homeit_0", &CNpcScript::mada_homeit_0},
	{"mada_homeit_1", &CNpcScript::mada_homeit_1},
	{"mada_homeit_2", &CNpcScript::mada_homeit_2},
	{"mada_homeit_3", &CNpcScript::mada_homeit_3},
	{"mada_homeit_4", &CNpcScript::mada_homeit_4},
	{"mada_homeit_5", &CNpcScript::mada_homeit_5},
	{"mada_homeit_6", &CNpcScript::mada_homeit_6},
	{"mada_homeit_7", &CNpcScript::mada_homeit_7},
	{"mada_homeit_8", &CNpcScript::mada_homeit_8},
	{"mada_homeit_9", &CNpcScript::mada_homeit_9},
	{"mada_homeit_10", &CNpcScript::mada_homeit_10},
	{"mada_horison_0", &CNpcScript::mada_horison_0},
	{"mada_horison_1", &CNpcScript::mada_horison_1},
	{"mada_horison_2", &CNpcScript::mada_horison_2},
	{"mada_horison_3", &CNpcScript::mada_horison_3},
	{"mada_horison_4", &CNpcScript::mada_horison_4},
	{"mada_horison_5", &CNpcScript::mada_horison_5},
	{"mada_horison_6", &CNpcScript::mada_horison_6},
	{"mada_horison_8", &CNpcScript::mada_horison_8},
	{"mada_horison_9", &CNpcScript::mada_horison_9},
	{"mada_horison_10", &CNpcScript::mada_horison_10},
	{"mada_jeperdy_0", &CNpcScript::mada_jeperdy_0},
	{"mada_jeperdy_1", &CNpcScript::mada_jeperdy_1},
	{"mada_jeperdy_2", &CNpcScript::mada_jeperdy_2},
	{"mada_jeperdy_3", &CNpcScript::mada_jeperdy_3},
	{"mada_jeperdy_4", &CNpcScript::mada_jeperdy_4},
	{"mada_jeperdy_5", &CNpcScript::mada_jeperdy_5},
	{"mada_jeperdy_6", &CNpcScript::mada_jeperdy_6},
	{"mada_jeperdy_8", &CNpcScript::mada_jeperdy_8},
	{"mada_jeperdy_9", &CNpcScript::mada_jeperdy_9},
	{"mada_jeperdy_10", &CNpcScript::mada_jeperdy_10},
	{"mada_jinitte_0", &CNpcScript::mada_jinitte_0},
	{"mada_jinitte_1", &CNpcScript::mada_jinitte_1},
	{"mada_jinitte_2", &CNpcScript::mada_jinitte_2},
	{"mada_jinitte_3", &CNpcScript::mada_jinitte_3},
	{"mada_jinitte_4", &CNpcScript::mada_jinitte_4},
	{"mada_jinitte_5", &CNpcScript::mada_jinitte_5},
	{"mada_jinitte_6", &CNpcScript::mada_jinitte_6},
	{"mada_jinitte_8", &CNpcScript::mada_jinitte_8},
	{"mada_jinitte_9", &CNpcScript::mada_jinitte_9},
	{"mada_jinitte_10", &CNpcScript::mada_jinitte_10},
	{"mada_kablloc_0", &CNpcScript::mada_kablloc_0},
	{"mada_kablloc_1", &CNpcScript::mada_kablloc_1},
	{"mada_kablloc_2", &CNpcScript::mada_kablloc_2},
	{"mada_kablloc_3", &CNpcScript::mada_kablloc_3},
	{"mada_kablloc_4", &CNpcScript::mada_kablloc_4},
	{"mada_kablloc_5", &CNpcScript::mada_kablloc_5},
	{"mada_kablloc_6", &CNpcScript::mada_kablloc_6},
	{"mada_kablloc_8", &CNpcScript::mada_kablloc_8},
	{"mada_kablloc_9", &CNpcScript::mada_kablloc_9},
	{"mada_kablloc_10", &CNpcScript::mada_kablloc_10},
	{"mada_kailreard_0", &CNpcScript::mada_kailreard_0},
	{"mada_kailreard_1", &CNpcScript::mada_kailreard_1},
	{"mada_kailreard_2", &CNpcScript::mada_kailreard_2},
	{"mada_kailreard_3", &CNpcScript::mada_kailreard_3},
	{"mada_kailreard_4", &CNpcScript::mada_kailreard_4},
	{"mada_kailreard_5", &CNpcScript::mada_kailreard_5},
	{"mada_kailreard_6", &CNpcScript::mada_kailreard_6},
	{"mada_kailreard_7", &CNpcScript::mada_kailreard_7},
	{"mada_kailreard_8", &CNpcScript::mada_kailreard_8},
	{"mada_kailreard_9", &CNpcScript::mada_kailreard_9},
	{"mada_kailreard_10", &CNpcScript::mada_kailreard_10},
	{"mada_karanduru_0", &CNpcScript::mada_karanduru_0},
	{"mada_karanduru_1", &CNpcScript::mada_karanduru_1},
	{"mada_karanduru_2", &CNpcScript::mada_karanduru_2},
	{"mada_karanduru_3", &CNpcScript::mada_karanduru_3},
	{"mada_karanduru_4", &CNpcScript::mada_karanduru_4},
	{"mada_karanduru_5", &CNpcScript::mada_karanduru_5},
	{"mada_karanduru_6", &CNpcScript::mada_karanduru_6},
	{"mada_karanduru_8", &CNpcScript::mada_karanduru_8},
	{"mada_karanduru_9", &CNpcScript::mada_karanduru_9},
	{"mada_karanduru_10", &CNpcScript::mada_karanduru_10},
	{"mada_krinton_0", &CNpcScript::mada_krinton_0},
	{"mada_krinton_1", &CNpcScript::mada_krinton_1},
	{"mada_krinton_2", &CNpcScript::mada_krinton_2},
	{"mada_krinton_3", &CNpcScript::mada_krinton_3},
	{"mada_krinton_4", &CNpcScript::mada_krinton_4},
	{"mada_krinton_5", &CNpcScript::mada_krinton_5},
	{"mada_krinton_6", &CNpcScript::mada_krinton_6},
	{"mada_krinton_7", &CNpcScript::mada_krinton_7},
	{"mada_krinton_8", &CNpcScript::mada_krinton_8},
	{"mada_krinton_9", &CNpcScript::mada_krinton_9},
	{"mada_krinton_10", &CNpcScript::mada_krinton_10},
	{"mada_laloa_0", &CNpcScript::mada_laloa_0},
	{"mada_laloa_1", &CNpcScript::mada_laloa_1},
	{"mada_laloa_2", &CNpcScript::mada_laloa_2},
	{"mada_laloa_3", &CNpcScript::mada_laloa_3},
	{"mada_laloa_4", &CNpcScript::mada_laloa_4},
	{"mada_laloa_5", &CNpcScript::mada_laloa_5},
	{"mada_laloa_6", &CNpcScript::mada_laloa_6},
	{"mada_laloa_8", &CNpcScript::mada_laloa_8},
	{"mada_laloa_9", &CNpcScript::mada_laloa_9},
	{"mada_laloa_10", &CNpcScript::mada_laloa_10},
	{"mada_liekyen_0", &CNpcScript::mada_liekyen_0},
	{"mada_liekyen_1", &CNpcScript::mada_liekyen_1},
	{"mada_liekyen_2", &CNpcScript::mada_liekyen_2},
	{"mada_liekyen_3", &CNpcScript::mada_liekyen_3},
	{"mada_liekyen_4", &CNpcScript::mada_liekyen_4},
	{"mada_liekyen_5", &CNpcScript::mada_liekyen_5},
	{"mada_liekyen_6", &CNpcScript::mada_liekyen_6},
	{"mada_liekyen_7", &CNpcScript::mada_liekyen_7},
	{"mada_liekyen_8", &CNpcScript::mada_liekyen_8},
	{"mada_liekyen_9", &CNpcScript::mada_liekyen_9},
	{"mada_liekyen_10", &CNpcScript::mada_liekyen_10},
	{"mada_lobiet_0", &CNpcScript::mada_lobiet_0},
	{"mada_lobiet_1", &CNpcScript::mada_lobiet_1},
	{"mada_lobiet_2", &CNpcScript::mada_lobiet_2},
	{"mada_lobiet_3", &CNpcScript::mada_lobiet_3},
	{"mada_lobiet_4", &CNpcScript::mada_lobiet_4},
	{"mada_lobiet_5", &CNpcScript::mada_lobiet_5},
	{"mada_lobiet_6", &CNpcScript::mada_lobiet_6},
	{"mada_lobiet_8", &CNpcScript::mada_lobiet_8},
	{"mada_lobiet_9", &CNpcScript::mada_lobiet_9},
	{"mada_lobiet_10", &CNpcScript::mada_lobiet_10},
	{"mada_lodelight_0", &CNpcScript::mada_lodelight_0},
	{"mada_lodelight_1", &CNpcScript::mada_lodelight_1},
	{"mada_lodelight_2", &CNpcScript::mada_lodelight_2},
	{"mada_lodelight_3", &CNpcScript::mada_lodelight_3},
	{"mada_lodelight_4", &CNpcScript::mada_lodelight_4},
	{"mada_lodelight_5", &CNpcScript::mada_lodelight_5},
	{"mada_lodelight_6", &CNpcScript::mada_lodelight_6},
	{"mada_lodelight_8", &CNpcScript::mada_lodelight_8},
	{"mada_lodelight_9", &CNpcScript::mada_lodelight_9},
	{"mada_lodelight_36", &CNpcScript::mada_lodelight_36},
	{"mada_lodelight_10", &CNpcScript::mada_lodelight_10},
	{"mada_lodelight2_0", &CNpcScript::mada_lodelight2_0},
	{"mada_lodelight2_1", &CNpcScript::mada_lodelight2_1},
	{"mada_lodelight2_2", &CNpcScript::mada_lodelight2_2},
	{"mada_lodelight2_3", &CNpcScript::mada_lodelight2_3},
	{"mada_lodelight2_4", &CNpcScript::mada_lodelight2_4},
	{"mada_lodelight2_5", &CNpcScript::mada_lodelight2_5},
	{"mada_lodelight2_6", &CNpcScript::mada_lodelight2_6},
	{"mada_lodelight2_8", &CNpcScript::mada_lodelight2_8},
	{"mada_lodelight2_9", &CNpcScript::mada_lodelight2_9},
	{"mada_lodelight2_36", &CNpcScript::mada_lodelight2_36},
	{"mada_lodelight2_10", &CNpcScript::mada_lodelight2_10},
	{"mada_lorein_0", &CNpcScript::mada_lorein_0},
	{"mada_lorein_1", &CNpcScript::mada_lorein_1},
	{"mada_lorein_2", &CNpcScript::mada_lorein_2},
	{"mada_lorein_3", &CNpcScript::mada_lorein_3},
	{"mada_lorein_4", &CNpcScript::mada_lorein_4},
	{"mada_lorein_5", &CNpcScript::mada_lorein_5},
	{"mada_lorein_6", &CNpcScript::mada_lorein_6},
	{"mada_lorein_7", &CNpcScript::mada_lorein_7},
	{"mada_lorein_8", &CNpcScript::mada_lorein_8},
	{"mada_lorein_9", &CNpcScript::mada_lorein_9},
	{"mada_lorein_10", &CNpcScript::mada_lorein_10},
	{"mada_lurif_0", &CNpcScript::mada_lurif_0},
	{"mada_lurif_1", &CNpcScript::mada_lurif_1},
	{"mada_lurif_2", &CNpcScript::mada_lurif_2},
	{"mada_lurif_3", &CNpcScript::mada_lurif_3},
	{"mada_lurif_4", &CNpcScript::mada_lurif_4},
	{"mada_lurif_5", &CNpcScript::mada_lurif_5},
	{"mada_lurif_6", &CNpcScript::mada_lurif_6},
	{"mada_lurif_7", &CNpcScript::mada_lurif_7},
	{"mada_lurif_8", &CNpcScript::mada_lurif_8},
	{"mada_lurif_9", &CNpcScript::mada_lurif_9},
	{"mada_lurif_10", &CNpcScript::mada_lurif_10},
	{"mada_nein_0", &CNpcScript::mada_nein_0},
	{"mada_nein_1", &CNpcScript::mada_nein_1},
	{"mada_nein_2", &CNpcScript::mada_nein_2},
	{"mada_nein_3", &CNpcScript::mada_nein_3},
	{"mada_nein_4", &CNpcScript::mada_nein_4},
	{"mada_nein_5", &CNpcScript::mada_nein_5},
	{"mada_nein_6", &CNpcScript::mada_nein_6},
	{"mada_nein_7", &CNpcScript::mada_nein_7},
	{"mada_nein_8", &CNpcScript::mada_nein_8},
	{"mada_nein_9", &CNpcScript::mada_nein_9},
	{"mada_nein_10", &CNpcScript::mada_nein_10},
	{"mada_ollien_0", &CNpcScript::mada_ollien_0},
	{"mada_ollien_1", &CNpcScript::mada_ollien_1},
	{"mada_ollien_2", &CNpcScript::mada_ollien_2},
	{"mada_ollien_3", &CNpcScript::mada_ollien_3},
	{"mada_ollien_4", &CNpcScript::mada_ollien_4},
	{"mada_ollien_5", &CNpcScript::mada_ollien_5},
	{"mada_ollien_6", &CNpcScript::mada_ollien_6},
	{"mada_ollien_8", &CNpcScript::mada_ollien_8},
	{"mada_ollien_9", &CNpcScript::mada_ollien_9},
	{"mada_ollien_37", &CNpcScript::mada_ollien_37},
	{"mada_ollien_10", &CNpcScript::mada_ollien_10},
	{"mada_pereb_0", &CNpcScript::mada_pereb_0},
	{"mada_pereb_1", &CNpcScript::mada_pereb_1},
	{"mada_pereb_2", &CNpcScript::mada_pereb_2},
	{"mada_pereb_3", &CNpcScript::mada_pereb_3},
	{"mada_pereb_4", &CNpcScript::mada_pereb_4},
	{"mada_pereb_5", &CNpcScript::mada_pereb_5},
	{"mada_pereb_6", &CNpcScript::mada_pereb_6},
	{"mada_pereb_8", &CNpcScript::mada_pereb_8},
	{"mada_pereb_9", &CNpcScript::mada_pereb_9},
	{"mada_pereb_10", &CNpcScript::mada_pereb_10},
	{"mada_phacham_0", &CNpcScript::mada_phacham_0},
	{"mada_phacham_1", &CNpcScript::mada_phacham_1},
	{"mada_phacham_2", &CNpcScript::mada_phacham_2},
	{"mada_phacham_3", &CNpcScript::mada_phacham_3},
	{"mada_phacham_4", &CNpcScript::mada_phacham_4},
	{"mada_phacham_5", &CNpcScript::mada_phacham_5},
	{"mada_phacham_6", &CNpcScript::mada_phacham_6},
	{"mada_phacham_8", &CNpcScript::mada_phacham_8},
	{"mada_phacham_9", &CNpcScript::mada_phacham_9},
	{"mada_phacham_10", &CNpcScript::mada_phacham_10},
	{"mada_pyre_0", &CNpcScript::mada_pyre_0},
	{"mada_pyre_1", &CNpcScript::mada_pyre_1},
	{"mada_pyre_2", &CNpcScript::mada_pyre_2},
	{"mada_pyre_3", &CNpcScript::mada_pyre_3},
	{"mada_pyre_4", &CNpcScript::mada_pyre_4},
	{"mada_pyre_5", &CNpcScript::mada_pyre_5},
	{"mada_pyre_6", &CNpcScript::mada_pyre_6},
	{"mada_pyre_7", &CNpcScript::mada_pyre_7},
	{"mada_pyre_8", &CNpcScript::mada_pyre_8},
	{"mada_pyre_9", &CNpcScript::mada_pyre_9},
	{"mada_pyre_10", &CNpcScript::mada_pyre_10},
	{"mada_rankashu_0", &CNpcScript::mada_rankashu_0},
	{"mada_rankashu_1", &CNpcScript::mada_rankashu_1},
	{"mada_rankashu_2", &CNpcScript::mada_rankashu_2},
	{"mada_rankashu_3", &CNpcScript::mada_rankashu_3},
	{"mada_rankashu_4", &CNpcScript::mada_rankashu_4},
	{"mada_rankashu_5", &CNpcScript::mada_rankashu_5},
	{"mada_rankashu_6", &CNpcScript::mada_rankashu_6},
	{"mada_rankashu_7", &CNpcScript::mada_rankashu_7},
	{"mada_rankashu_8", &CNpcScript::mada_rankashu_8},
	{"mada_rankashu_12", &CNpcScript::mada_rankashu_12},
	{"mada_rankashu_38", &CNpcScript::mada_rankashu_38},
	{"mada_rankashu_10", &CNpcScript::mada_rankashu_10},
	{"mada_rankashu_39", &CNpcScript::mada_rankashu_39},
	{"mada_remine_0", &CNpcScript::mada_remine_0},
	{"mada_remine_1", &CNpcScript::mada_remine_1},
	{"mada_remine_2", &CNpcScript::mada_remine_2},
	{"mada_remine_3", &CNpcScript::mada_remine_3},
	{"mada_remine_4", &CNpcScript::mada_remine_4},
	{"mada_remine_5", &CNpcScript::mada_remine_5},
	{"mada_remine_6", &CNpcScript::mada_remine_6},
	{"mada_remine_8", &CNpcScript::mada_remine_8},
	{"mada_remine_9", &CNpcScript::mada_remine_9},
	{"mada_remine_10", &CNpcScript::mada_remine_10},
	{"mada_ride_0", &CNpcScript::mada_ride_0},
	{"mada_ride_1", &CNpcScript::mada_ride_1},
	{"mada_ride_2", &CNpcScript::mada_ride_2},
	{"mada_ride_3", &CNpcScript::mada_ride_3},
	{"mada_ride_4", &CNpcScript::mada_ride_4},
	{"mada_ride_5", &CNpcScript::mada_ride_5},
	{"mada_ride_6", &CNpcScript::mada_ride_6},
	{"mada_ride_8", &CNpcScript::mada_ride_8},
	{"mada_ride_9", &CNpcScript::mada_ride_9},
	{"mada_ride_10", &CNpcScript::mada_ride_10},
	{"mada_romvoette_0", &CNpcScript::mada_romvoette_0},
	{"mada_romvoette_1", &CNpcScript::mada_romvoette_1},
	{"mada_romvoette_2", &CNpcScript::mada_romvoette_2},
	{"mada_romvoette_3", &CNpcScript::mada_romvoette_3},
	{"mada_romvoette_4", &CNpcScript::mada_romvoette_4},
	{"mada_romvoette_5", &CNpcScript::mada_romvoette_5},
	{"mada_romvoette_6", &CNpcScript::mada_romvoette_6},
	{"mada_romvoette_8", &CNpcScript::mada_romvoette_8},
	{"mada_romvoette_9", &CNpcScript::mada_romvoette_9},
	{"mada_romvoette_10", &CNpcScript::mada_romvoette_10},
	{"mada_roocky_0", &CNpcScript::mada_roocky_0},
	{"mada_roocky_1", &CNpcScript::mada_roocky_1},
	{"mada_roocky_2", &CNpcScript::mada_roocky_2},
	{"mada_roocky_3", &CNpcScript::mada_roocky_3},
	{"mada_roocky_4", &CNpcScript::mada_roocky_4},
	{"mada_roocky_5", &CNpcScript::mada_roocky_5},
	{"mada_roocky_6", &CNpcScript::mada_roocky_6},
	{"mada_roocky_8", &CNpcScript::mada_roocky_8},
	{"mada_roocky_9", &CNpcScript::mada_roocky_9},
	{"mada_roocky_10", &CNpcScript::mada_roocky_10},
	{"mada_rupim_0", &CNpcScript::mada_rupim_0},
	{"mada_rupim_1", &CNpcScript::mada_rupim_1},
	{"mada_rupim_2", &CNpcScript::mada_rupim_2},
	{"mada_rupim_3", &CNpcScript::mada_rupim_3},
	{"mada_rupim_4", &CNpcScript::mada_rupim_4},
	{"mada_rupim_5", &CNpcScript::mada_rupim_5},
	{"mada_rupim_6", &CNpcScript::mada_rupim_6},
	{"mada_rupim_7", &CNpcScript::mada_rupim_7},
	{"mada_rupim_8", &CNpcScript::mada_rupim_8},
	{"mada_rupim_9", &CNpcScript::mada_rupim_9},
	{"mada_rupim_10", &CNpcScript::mada_rupim_10},
	{"mada_sencyrit_0", &CNpcScript::mada_sencyrit_0},
	{"mada_sencyrit_1", &CNpcScript::mada_sencyrit_1},
	{"mada_sencyrit_2", &CNpcScript::mada_sencyrit_2},
	{"mada_sencyrit_3", &CNpcScript::mada_sencyrit_3},
	{"mada_sencyrit_4", &CNpcScript::mada_sencyrit_4},
	{"mada_sencyrit_5", &CNpcScript::mada_sencyrit_5},
	{"mada_sencyrit_6", &CNpcScript::mada_sencyrit_6},
	{"mada_sencyrit_8", &CNpcScript::mada_sencyrit_8},
	{"mada_sencyrit_9", &CNpcScript::mada_sencyrit_9},
	{"mada_sencyrit_10", &CNpcScript::mada_sencyrit_10},
	{"mada_shyniff_0", &CNpcScript::mada_shyniff_0},
	{"mada_shyniff_1", &CNpcScript::mada_shyniff_1},
	{"mada_shyniff_2", &CNpcScript::mada_shyniff_2},
	{"mada_shyniff_3", &CNpcScript::mada_shyniff_3},
	{"mada_shyniff_4", &CNpcScript::mada_shyniff_4},
	{"mada_shyniff_5", &CNpcScript::mada_shyniff_5},
	{"mada_shyniff_6", &CNpcScript::mada_shyniff_6},
	{"mada_shyniff_8", &CNpcScript::mada_shyniff_8},
	{"mada_shyniff_9", &CNpcScript::mada_shyniff_9},
	{"mada_shyniff_10", &CNpcScript::mada_shyniff_10},
	{"mada_stima_0", &CNpcScript::mada_stima_0},
	{"mada_stima_1", &CNpcScript::mada_stima_1},
	{"mada_stima_2", &CNpcScript::mada_stima_2},
	{"mada_stima_3", &CNpcScript::mada_stima_3},
	{"mada_stima_4", &CNpcScript::mada_stima_4},
	{"mada_stima_5", &CNpcScript::mada_stima_5},
	{"mada_stima_6", &CNpcScript::mada_stima_6},
	{"mada_stima_8", &CNpcScript::mada_stima_8},
	{"mada_stima_9", &CNpcScript::mada_stima_9},
	{"mada_stima_10", &CNpcScript::mada_stima_10},
	{"mada_tailer_0", &CNpcScript::mada_tailer_0},
	{"mada_tailer_1", &CNpcScript::mada_tailer_1},
	{"mada_tailer_2", &CNpcScript::mada_tailer_2},
	{"mada_tailer_3", &CNpcScript::mada_tailer_3},
	{"mada_tailer_4", &CNpcScript::mada_tailer_4},
	{"mada_tailer_5", &CNpcScript::mada_tailer_5},
	{"mada_tailer_6", &CNpcScript::mada_tailer_6},
	{"mada_tailer_7", &CNpcScript::mada_tailer_7},
	{"mada_tailer_8", &CNpcScript::mada_tailer_8},
	{"mada_tailer_9", &CNpcScript::mada_tailer_9},
	{"mada_tailer_10", &CNpcScript::mada_tailer_10},
	{"mada_tandy_0", &CNpcScript::mada_tandy_0},
	{"mada_tandy_1", &CNpcScript::mada_tandy_1},
	{"mada_tandy_2", &CNpcScript::mada_tandy_2},
	{"mada_tandy_3", &CNpcScript::mada_tandy_3},
	{"mada_tandy_4", &CNpcScript::mada_tandy_4},
	{"mada_tandy_5", &CNpcScript::mada_tandy_5},
	{"mada_tandy_6", &CNpcScript::mada_tandy_6},
	{"mada_tandy_8", &CNpcScript::mada_tandy_8},
	{"mada_tandy_9", &CNpcScript::mada_tandy_9},
	{"mada_tandy_10", &CNpcScript::mada_tandy_10},
	{"mada_wendien_0", &CNpcScript::mada_wendien_0},
	{"mada_wendien_1", &CNpcScript::mada_wendien_1},
	{"mada_wendien_2", &CNpcScript::mada_wendien_2},
	{"mada_wendien_3", &CNpcScript::mada_wendien_3},
	{"mada_wendien_4", &CNpcScript::mada_wendien_4},
	{"mada_wendien_5", &CNpcScript::mada_wendien_5},
	{"mada_wendien_6", &CNpcScript::mada_wendien_6},
	{"mada_wendien_8", &CNpcScript::mada_wendien_8},
	{"mada_wendien_9", &CNpcScript::mada_wendien_9},
	{"mada_wendien_10", &CNpcScript::mada_wendien_10},
	{"mafl_andy_0", &CNpcScript::mafl_andy_0},
	{"mafl_andy_1", &CNpcScript::mafl_andy_1},
	{"mafl_andy_2", &CNpcScript::mafl_andy_2},
	{"mafl_andy_3", &CNpcScript::mafl_andy_3},
	{"mafl_andy_4", &CNpcScript::mafl_andy_4},
	{"mafl_andy_5", &CNpcScript::mafl_andy_5},
	{"mafl_andy_6", &CNpcScript::mafl_andy_6},
	{"mafl_andy_8", &CNpcScript::mafl_andy_8},
	{"mafl_andy_9", &CNpcScript::mafl_andy_9},
	{"mafl_andy_10", &CNpcScript::mafl_andy_10},
	{"mafl_aramy_0", &CNpcScript::mafl_aramy_0},
	{"mafl_aramy_1", &CNpcScript::mafl_aramy_1},
	{"mafl_aramy_2", &CNpcScript::mafl_aramy_2},
	{"mafl_aramy_3", &CNpcScript::mafl_aramy_3},
	{"mafl_aramy_4", &CNpcScript::mafl_aramy_4},
	{"mafl_aramy_5", &CNpcScript::mafl_aramy_5},
	{"mafl_aramy_6", &CNpcScript::mafl_aramy_6},
	{"mafl_aramy_8", &CNpcScript::mafl_aramy_8},
	{"mafl_aramy_9", &CNpcScript::mafl_aramy_9},
	{"mafl_aramy_10", &CNpcScript::mafl_aramy_10},
	{"mafl_ata_0", &CNpcScript::mafl_ata_0},
	{"mafl_ata_1", &CNpcScript::mafl_ata_1},
	{"mafl_ata_2", &CNpcScript::mafl_ata_2},
	{"mafl_ata_3", &CNpcScript::mafl_ata_3},
	{"mafl_ata_4", &CNpcScript::mafl_ata_4},
	{"mafl_ata_5", &CNpcScript::mafl_ata_5},
	{"mafl_ata_6", &CNpcScript::mafl_ata_6},
	{"mafl_ata_8", &CNpcScript::mafl_ata_8},
	{"mafl_ata_9", &CNpcScript::mafl_ata_9},
	{"mafl_ata_10", &CNpcScript::mafl_ata_10},
	{"mafl_atto_0", &CNpcScript::mafl_atto_0},
	{"mafl_atto_1", &CNpcScript::mafl_atto_1},
	{"mafl_atto_2", &CNpcScript::mafl_atto_2},
	{"mafl_atto_3", &CNpcScript::mafl_atto_3},
	{"mafl_atto_4", &CNpcScript::mafl_atto_4},
	{"mafl_atto_5", &CNpcScript::mafl_atto_5},
	{"mafl_atto_6", &CNpcScript::mafl_atto_6},
	{"mafl_atto_8", &CNpcScript::mafl_atto_8},
	{"mafl_atto_9", &CNpcScript::mafl_atto_9},
	{"mafl_atto_10", &CNpcScript::mafl_atto_10},
	{"mafl_boboko_0", &CNpcScript::mafl_boboko_0},
	{"mafl_boboko_1", &CNpcScript::mafl_boboko_1},
	{"mafl_boboko_2", &CNpcScript::mafl_boboko_2},
	{"mafl_boboko_3", &CNpcScript::mafl_boboko_3},
	{"mafl_boboko_4", &CNpcScript::mafl_boboko_4},
	{"mafl_boboko_5", &CNpcScript::mafl_boboko_5},
	{"mafl_boboko_6", &CNpcScript::mafl_boboko_6},
	{"mafl_boboko_8", &CNpcScript::mafl_boboko_8},
	{"mafl_boboko_9", &CNpcScript::mafl_boboko_9},
	{"mafl_boboko_10", &CNpcScript::mafl_boboko_10},
	{"mafl_boboku_0", &CNpcScript::mafl_boboku_0},
	{"mafl_boboku_1", &CNpcScript::mafl_boboku_1},
	{"mafl_boboku_2", &CNpcScript::mafl_boboku_2},
	{"mafl_boboku_3", &CNpcScript::mafl_boboku_3},
	{"mafl_boboku_4", &CNpcScript::mafl_boboku_4},
	{"mafl_boboku_5", &CNpcScript::mafl_boboku_5},
	{"mafl_boboku_6", &CNpcScript::mafl_boboku_6},
	{"mafl_boboku_8", &CNpcScript::mafl_boboku_8},
	{"mafl_boboku_9", &CNpcScript::mafl_boboku_9},
	{"mafl_boboku_10", &CNpcScript::mafl_boboku_10},
	{"mafl_buruto_0", &CNpcScript::mafl_buruto_0},
	{"mafl_buruto_1", &CNpcScript::mafl_buruto_1},
	{"mafl_buruto_2", &CNpcScript::mafl_buruto_2},
	{"mafl_buruto_3", &CNpcScript::mafl_buruto_3},
	{"mafl_buruto_4", &CNpcScript::mafl_buruto_4},
	{"mafl_buruto_5", &CNpcScript::mafl_buruto_5},
	{"mafl_buruto_6", &CNpcScript::mafl_buruto_6},
	{"mafl_buruto_8", &CNpcScript::mafl_buruto_8},
	{"mafl_buruto_9", &CNpcScript::mafl_buruto_9},
	{"mafl_buruto_10", &CNpcScript::mafl_buruto_10},
	{"mafl_chichi_0", &CNpcScript::mafl_chichi_0},
	{"mafl_chichi_1", &CNpcScript::mafl_chichi_1},
	{"mafl_chichi_2", &CNpcScript::mafl_chichi_2},
	{"mafl_chichi_3", &CNpcScript::mafl_chichi_3},
	{"mafl_chichi_4", &CNpcScript::mafl_chichi_4},
	{"mafl_chichi_5", &CNpcScript::mafl_chichi_5},
	{"mafl_chichi_6", &CNpcScript::mafl_chichi_6},
	{"mafl_chichi_8", &CNpcScript::mafl_chichi_8},
	{"mafl_chichi_9", &CNpcScript::mafl_chichi_9},
	{"mafl_chichi_10", &CNpcScript::mafl_chichi_10},
	{"mafl_chorus1_0", &CNpcScript::mafl_chorus1_0},
	{"mafl_chorus1_1", &CNpcScript::mafl_chorus1_1},
	{"mafl_chorus1_2", &CNpcScript::mafl_chorus1_2},
	{"mafl_chorus1_3", &CNpcScript::mafl_chorus1_3},
	{"mafl_chorus1_4", &CNpcScript::mafl_chorus1_4},
	{"mafl_chorus1_5", &CNpcScript::mafl_chorus1_5},
	{"mafl_chorus1_6", &CNpcScript::mafl_chorus1_6},
	{"mafl_chorus1_8", &CNpcScript::mafl_chorus1_8},
	{"mafl_chorus1_10", &CNpcScript::mafl_chorus1_10},
	{"mafl_chorus2_0", &CNpcScript::mafl_chorus2_0},
	{"mafl_chorus2_1", &CNpcScript::mafl_chorus2_1},
	{"mafl_chorus2_2", &CNpcScript::mafl_chorus2_2},
	{"mafl_chorus2_3", &CNpcScript::mafl_chorus2_3},
	{"mafl_chorus2_4", &CNpcScript::mafl_chorus2_4},
	{"mafl_chorus2_5", &CNpcScript::mafl_chorus2_5},
	{"mafl_chorus2_6", &CNpcScript::mafl_chorus2_6},
	{"mafl_chorus2_8", &CNpcScript::mafl_chorus2_8},
	{"mafl_chorus2_10", &CNpcScript::mafl_chorus2_10},
	{"mafl_chorus3_0", &CNpcScript::mafl_chorus3_0},
	{"mafl_chorus3_1", &CNpcScript::mafl_chorus3_1},
	{"mafl_chorus3_2", &CNpcScript::mafl_chorus3_2},
	{"mafl_chorus3_3", &CNpcScript::mafl_chorus3_3},
	{"mafl_chorus3_4", &CNpcScript::mafl_chorus3_4},
	{"mafl_chorus3_5", &CNpcScript::mafl_chorus3_5},
	{"mafl_chorus3_6", &CNpcScript::mafl_chorus3_6},
	{"mafl_chorus3_8", &CNpcScript::mafl_chorus3_8},
	{"mafl_chorus3_10", &CNpcScript::mafl_chorus3_10},
	{"mafl_chorus4_0", &CNpcScript::mafl_chorus4_0},
	{"mafl_chorus4_1", &CNpcScript::mafl_chorus4_1},
	{"mafl_chorus4_2", &CNpcScript::mafl_chorus4_2},
	{"mafl_chorus4_3", &CNpcScript::mafl_chorus4_3},
	{"mafl_chorus4_4", &CNpcScript::mafl_chorus4_4},
	{"mafl_chorus4_5", &CNpcScript::mafl_chorus4_5},
	{"mafl_chorus4_6", &CNpcScript::mafl_chorus4_6},
	{"mafl_chorus4_8", &CNpcScript::mafl_chorus4_8},
	{"mafl_chorus4_10", &CNpcScript::mafl_chorus4_10},
	{"mafl_clamb_0", &CNpcScript::mafl_clamb_0},
	{"mafl_clamb_1", &CNpcScript::mafl_clamb_1},
	{"mafl_clamb_2", &CNpcScript::mafl_clamb_2},
	{"mafl_clamb_3", &CNpcScript::mafl_clamb_3},
	{"mafl_clamb_4", &CNpcScript::mafl_clamb_4},
	{"mafl_clamb_5", &CNpcScript::mafl_clamb_5},
	{"mafl_clamb_6", &CNpcScript::mafl_clamb_6},
	{"mafl_clamb_8", &CNpcScript::mafl_clamb_8},
	{"mafl_clamb_9", &CNpcScript::mafl_clamb_9},
	{"mafl_clamb_10", &CNpcScript::mafl_clamb_10},
	{"mafl_cuarine_0", &CNpcScript::mafl_cuarine_0},
	{"mafl_cuarine_1", &CNpcScript::mafl_cuarine_1},
	{"mafl_cuarine_2", &CNpcScript::mafl_cuarine_2},
	{"mafl_cuarine_3", &CNpcScript::mafl_cuarine_3},
	{"mafl_cuarine_4", &CNpcScript::mafl_cuarine_4},
	{"mafl_cuarine_5", &CNpcScript::mafl_cuarine_5},
	{"mafl_cuarine_6", &CNpcScript::mafl_cuarine_6},
	{"mafl_cuarine_8", &CNpcScript::mafl_cuarine_8},
	{"mafl_cuarine_9", &CNpcScript::mafl_cuarine_9},
	{"mafl_cuarine_10", &CNpcScript::mafl_cuarine_10},
	{"mafl_cuzrill_0", &CNpcScript::mafl_cuzrill_0},
	{"mafl_cuzrill_1", &CNpcScript::mafl_cuzrill_1},
	{"mafl_cuzrill_2", &CNpcScript::mafl_cuzrill_2},
	{"mafl_cuzrill_3", &CNpcScript::mafl_cuzrill_3},
	{"mafl_cuzrill_4", &CNpcScript::mafl_cuzrill_4},
	{"mafl_cuzrill_5", &CNpcScript::mafl_cuzrill_5},
	{"mafl_cuzrill_6", &CNpcScript::mafl_cuzrill_6},
	{"mafl_cuzrill_8", &CNpcScript::mafl_cuzrill_8},
	{"mafl_cuzrill_9", &CNpcScript::mafl_cuzrill_9},
	{"mafl_cuzrill_10", &CNpcScript::mafl_cuzrill_10},
	{"mafl_dior_0", &CNpcScript::mafl_dior_0},
	{"mafl_dior_1", &CNpcScript::mafl_dior_1},
	{"mafl_dior_2", &CNpcScript::mafl_dior_2},
	{"mafl_dior_3", &CNpcScript::mafl_dior_3},
	{"mafl_dior_4", &CNpcScript::mafl_dior_4},
	{"mafl_dior_5", &CNpcScript::mafl_dior_5},
	{"mafl_dior_6", &CNpcScript::mafl_dior_6},
	{"mafl_dior_8", &CNpcScript::mafl_dior_8},
	{"mafl_dior_9", &CNpcScript::mafl_dior_9},
	{"mafl_dior_40", &CNpcScript::mafl_dior_40},
	{"mafl_dior_41", &CNpcScript::mafl_dior_41},
	{"mafl_dior_10", &CNpcScript::mafl_dior_10},
	{"mafl_domek_0", &CNpcScript::mafl_domek_0},
	{"mafl_domek_1", &CNpcScript::mafl_domek_1},
	{"mafl_domek_2", &CNpcScript::mafl_domek_2},
	{"mafl_domek_3", &CNpcScript::mafl_domek_3},
	{"mafl_domek_4", &CNpcScript::mafl_domek_4},
	{"mafl_domek_5", &CNpcScript::mafl_domek_5},
	{"mafl_domek_6", &CNpcScript::mafl_domek_6},
	{"mafl_domek_8", &CNpcScript::mafl_domek_8},
	{"mafl_domek_9", &CNpcScript::mafl_domek_9},
	{"mafl_domek_10", &CNpcScript::mafl_domek_10},
	{"mafl_donaris_0", &CNpcScript::mafl_donaris_0},
	{"mafl_donaris_1", &CNpcScript::mafl_donaris_1},
	{"mafl_donaris_2", &CNpcScript::mafl_donaris_2},
	{"mafl_donaris_3", &CNpcScript::mafl_donaris_3},
	{"mafl_donaris_4", &CNpcScript::mafl_donaris_4},
	{"mafl_donaris_5", &CNpcScript::mafl_donaris_5},
	{"mafl_donaris_6", &CNpcScript::mafl_donaris_6},
	{"mafl_donaris_7", &CNpcScript::mafl_donaris_7},
	{"mafl_donaris_8", &CNpcScript::mafl_donaris_8},
	{"mafl_donaris_9", &CNpcScript::mafl_donaris_9},
	{"mafl_donaris_10", &CNpcScript::mafl_donaris_10},
	{"mafl_drestly_0", &CNpcScript::mafl_drestly_0},
	{"mafl_drestly_1", &CNpcScript::mafl_drestly_1},
	{"mafl_drestly_2", &CNpcScript::mafl_drestly_2},
	{"mafl_drestly_3", &CNpcScript::mafl_drestly_3},
	{"mafl_drestly_4", &CNpcScript::mafl_drestly_4},
	{"mafl_drestly_5", &CNpcScript::mafl_drestly_5},
	{"mafl_drestly_6", &CNpcScript::mafl_drestly_6},
	{"mafl_drestly_8", &CNpcScript::mafl_drestly_8},
	{"mafl_drestly_9", &CNpcScript::mafl_drestly_9},
	{"mafl_drestly_10", &CNpcScript::mafl_drestly_10},
	{"mafl_elic_0", &CNpcScript::mafl_elic_0},
	{"mafl_elic_1", &CNpcScript::mafl_elic_1},
	{"mafl_elic_2", &CNpcScript::mafl_elic_2},
	{"mafl_elic_3", &CNpcScript::mafl_elic_3},
	{"mafl_elic_4", &CNpcScript::mafl_elic_4},
	{"mafl_elic_5", &CNpcScript::mafl_elic_5},
	{"mafl_elic_6", &CNpcScript::mafl_elic_6},
	{"mafl_elic_8", &CNpcScript::mafl_elic_8},
	{"mafl_elic_9", &CNpcScript::mafl_elic_9},
	{"mafl_elic_10", &CNpcScript::mafl_elic_10},
	{"mafl_eoners_0", &CNpcScript::mafl_eoners_0},
	{"mafl_eoners_1", &CNpcScript::mafl_eoners_1},
	{"mafl_eoners_2", &CNpcScript::mafl_eoners_2},
	{"mafl_eoners_3", &CNpcScript::mafl_eoners_3},
	{"mafl_eoners_4", &CNpcScript::mafl_eoners_4},
	{"mafl_eoners_5", &CNpcScript::mafl_eoners_5},
	{"mafl_eoners_6", &CNpcScript::mafl_eoners_6},
	{"mafl_eoners_8", &CNpcScript::mafl_eoners_8},
	{"mafl_eoners_9", &CNpcScript::mafl_eoners_9},
	{"mafl_eoners_10", &CNpcScript::mafl_eoners_10},
	{"mafl_flamayor_0", &CNpcScript::mafl_flamayor_0},
	{"mafl_flamayor_1", &CNpcScript::mafl_flamayor_1},
	{"mafl_flamayor_2", &CNpcScript::mafl_flamayor_2},
	{"mafl_flamayor_3", &CNpcScript::mafl_flamayor_3},
	{"mafl_flamayor_4", &CNpcScript::mafl_flamayor_4},
	{"mafl_flamayor_5", &CNpcScript::mafl_flamayor_5},
	{"mafl_flamayor_6", &CNpcScript::mafl_flamayor_6},
	{"mafl_flamayor_8", &CNpcScript::mafl_flamayor_8},
	{"mafl_flamayor_9", &CNpcScript::mafl_flamayor_9},
	{"mafl_flamayor_10", &CNpcScript::mafl_flamayor_10},
	{"mafl_flraness_0", &CNpcScript::mafl_flraness_0},
	{"mafl_flraness_1", &CNpcScript::mafl_flraness_1},
	{"mafl_flraness_2", &CNpcScript::mafl_flraness_2},
	{"mafl_flraness_3", &CNpcScript::mafl_flraness_3},
	{"mafl_flraness_4", &CNpcScript::mafl_flraness_4},
	{"mafl_flraness_5", &CNpcScript::mafl_flraness_5},
	{"mafl_flraness_6", &CNpcScript::mafl_flraness_6},
	{"mafl_flraness_7", &CNpcScript::mafl_flraness_7},
	{"mafl_flraness_8", &CNpcScript::mafl_flraness_8},
	{"mafl_flraness_10", &CNpcScript::mafl_flraness_10},
	{"mafl_furan_0", &CNpcScript::mafl_furan_0},
	{"mafl_furan_1", &CNpcScript::mafl_furan_1},
	{"mafl_furan_2", &CNpcScript::mafl_furan_2},
	{"mafl_furan_3", &CNpcScript::mafl_furan_3},
	{"mafl_furan_4", &CNpcScript::mafl_furan_4},
	{"mafl_furan_5", &CNpcScript::mafl_furan_5},
	{"mafl_furan_6", &CNpcScript::mafl_furan_6},
	{"mafl_furan_7", &CNpcScript::mafl_furan_7},
	{"mafl_furan_8", &CNpcScript::mafl_furan_8},
	{"mafl_furan_9", &CNpcScript::mafl_furan_9},
	{"mafl_furan_10", &CNpcScript::mafl_furan_10},
	{"mafl_gergantes_0", &CNpcScript::mafl_gergantes_0},
	{"mafl_gergantes_1", &CNpcScript::mafl_gergantes_1},
	{"mafl_gergantes_2", &CNpcScript::mafl_gergantes_2},
	{"mafl_gergantes_3", &CNpcScript::mafl_gergantes_3},
	{"mafl_gergantes_4", &CNpcScript::mafl_gergantes_4},
	{"mafl_gergantes_5", &CNpcScript::mafl_gergantes_5},
	{"mafl_gergantes_6", &CNpcScript::mafl_gergantes_6},
	{"mafl_gergantes_8", &CNpcScript::mafl_gergantes_8},
	{"mafl_gergantes_9", &CNpcScript::mafl_gergantes_9},
	{"mafl_gergantes_10", &CNpcScript::mafl_gergantes_10},
	{"mafl_ghalade_0", &CNpcScript::mafl_ghalade_0},
	{"mafl_ghalade_1", &CNpcScript::mafl_ghalade_1},
	{"mafl_ghalade_2", &CNpcScript::mafl_ghalade_2},
	{"mafl_ghalade_3", &CNpcScript::mafl_ghalade_3},
	{"mafl_ghalade_4", &CNpcScript::mafl_ghalade_4},
	{"mafl_ghalade_5", &CNpcScript::mafl_ghalade_5},
	{"mafl_ghalade_6", &CNpcScript::mafl_ghalade_6},
	{"mafl_ghalade_8", &CNpcScript::mafl_ghalade_8},
	{"mafl_ghalade_9", &CNpcScript::mafl_ghalade_9},
	{"mafl_ghalade_10", &CNpcScript::mafl_ghalade_10},
	{"mafl_goripeg_0", &CNpcScript::mafl_goripeg_0},
	{"mafl_goripeg_1", &CNpcScript::mafl_goripeg_1},
	{"mafl_goripeg_2", &CNpcScript::mafl_goripeg_2},
	{"mafl_goripeg_3", &CNpcScript::mafl_goripeg_3},
	{"mafl_goripeg_4", &CNpcScript::mafl_goripeg_4},
	{"mafl_goripeg_5", &CNpcScript::mafl_goripeg_5},
	{"mafl_goripeg_6", &CNpcScript::mafl_goripeg_6},
	{"mafl_goripeg_8", &CNpcScript::mafl_goripeg_8},
	{"mafl_goripeg_9", &CNpcScript::mafl_goripeg_9},
	{"mafl_goripeg_10", &CNpcScript::mafl_goripeg_10},
	{"mafl_gornus_0", &CNpcScript::mafl_gornus_0},
	{"mafl_gornus_1", &CNpcScript::mafl_gornus_1},
	{"mafl_gornus_2", &CNpcScript::mafl_gornus_2},
	{"mafl_gornus_3", &CNpcScript::mafl_gornus_3},
	{"mafl_gornus_4", &CNpcScript::mafl_gornus_4},
	{"mafl_gornus_5", &CNpcScript::mafl_gornus_5},
	{"mafl_gornus_6", &CNpcScript::mafl_gornus_6},
	{"mafl_gornus_8", &CNpcScript::mafl_gornus_8},
	{"mafl_gornus_9", &CNpcScript::mafl_gornus_9},
	{"mafl_gornus_10", &CNpcScript::mafl_gornus_10},
	{"mafl_guabrill_0", &CNpcScript::mafl_guabrill_0},
	{"mafl_guabrill_1", &CNpcScript::mafl_guabrill_1},
	{"mafl_guabrill_2", &CNpcScript::mafl_guabrill_2},
	{"mafl_guabrill_3", &CNpcScript::mafl_guabrill_3},
	{"mafl_guabrill_4", &CNpcScript::mafl_guabrill_4},
	{"mafl_guabrill_5", &CNpcScript::mafl_guabrill_5},
	{"mafl_guabrill_6", &CNpcScript::mafl_guabrill_6},
	{"mafl_guabrill_8", &CNpcScript::mafl_guabrill_8},
	{"mafl_guabrill_9", &CNpcScript::mafl_guabrill_9},
	{"mafl_guabrill_10", &CNpcScript::mafl_guabrill_10},
	{"mafl_guildwarser_0", &CNpcScript::mafl_guildwarser_0},
	{"mafl_guildwarser_1", &CNpcScript::mafl_guildwarser_1},
	{"mafl_guildwarser_2", &CNpcScript::mafl_guildwarser_2},
	{"mafl_guildwarser_3", &CNpcScript::mafl_guildwarser_3},
	{"mafl_guildwarser_4", &CNpcScript::mafl_guildwarser_4},
	{"mafl_guildwarser_5", &CNpcScript::mafl_guildwarser_5},
	{"mafl_guildwarser_6", &CNpcScript::mafl_guildwarser_6},
	{"mafl_guildwarser_7", &CNpcScript::mafl_guildwarser_7},
	{"mafl_guildwarser_8", &CNpcScript::mafl_guildwarser_8},
	{"mafl_guildwarser_9", &CNpcScript::mafl_guildwarser_9},
	{"mafl_guildwarser_10", &CNpcScript::mafl_guildwarser_10},
	{"mafl_hastan_0", &CNpcScript::mafl_hastan_0},
	{"mafl_hastan_1", &CNpcScript::mafl_hastan_1},
	{"mafl_hastan_2", &CNpcScript::mafl_hastan_2},
	{"mafl_hastan_3", &CNpcScript::mafl_hastan_3},
	{"mafl_hastan_4", &CNpcScript::mafl_hastan_4},
	{"mafl_hastan_5", &CNpcScript::mafl_hastan_5},
	{"mafl_hastan_6", &CNpcScript::mafl_hastan_6},
	{"mafl_hastan_8", &CNpcScript::mafl_hastan_8},
	{"mafl_hastan_9", &CNpcScript::mafl_hastan_9},
	{"mafl_hastan_10", &CNpcScript::mafl_hastan_10},
	{"mafl_himadel_0", &CNpcScript::mafl_himadel_0},
	{"mafl_himadel_1", &CNpcScript::mafl_himadel_1},
	{"mafl_himadel_2", &CNpcScript::mafl_himadel_2},
	{"mafl_himadel_3", &CNpcScript::mafl_himadel_3},
	{"mafl_himadel_4", &CNpcScript::mafl_himadel_4},
	{"mafl_himadel_5", &CNpcScript::mafl_himadel_5},
	{"mafl_himadel_6", &CNpcScript::mafl_himadel_6},
	{"mafl_himadel_8", &CNpcScript::mafl_himadel_8},
	{"mafl_himadel_9", &CNpcScript::mafl_himadel_9},
	{"mafl_himadel_10", &CNpcScript::mafl_himadel_10},
	{"mafl_hormes_0", &CNpcScript::mafl_hormes_0},
	{"mafl_hormes_1", &CNpcScript::mafl_hormes_1},
	{"mafl_hormes_2", &CNpcScript::mafl_hormes_2},
	{"mafl_hormes_3", &CNpcScript::mafl_hormes_3},
	{"mafl_hormes_4", &CNpcScript::mafl_hormes_4},
	{"mafl_hormes_5", &CNpcScript::mafl_hormes_5},
	{"mafl_hormes_6", &CNpcScript::mafl_hormes_6},
	{"mafl_hormes_8", &CNpcScript::mafl_hormes_8},
	{"mafl_hormes_9", &CNpcScript::mafl_hormes_9},
	{"mafl_hormes_10", &CNpcScript::mafl_hormes_10},
	{"mafl_hyuit_0", &CNpcScript::mafl_hyuit_0},
	{"mafl_hyuit_1", &CNpcScript::mafl_hyuit_1},
	{"mafl_hyuit_2", &CNpcScript::mafl_hyuit_2},
	{"mafl_hyuit_3", &CNpcScript::mafl_hyuit_3},
	{"mafl_hyuit_4", &CNpcScript::mafl_hyuit_4},
	{"mafl_hyuit_5", &CNpcScript::mafl_hyuit_5},
	{"mafl_hyuit_6", &CNpcScript::mafl_hyuit_6},
	{"mafl_hyuit_8", &CNpcScript::mafl_hyuit_8},
	{"mafl_hyuit_9", &CNpcScript::mafl_hyuit_9},
	{"mafl_hyuit_10", &CNpcScript::mafl_hyuit_10},
	{"mafl_is_0", &CNpcScript::mafl_is_0},
	{"mafl_is_1", &CNpcScript::mafl_is_1},
	{"mafl_is_2", &CNpcScript::mafl_is_2},
	{"mafl_is_3", &CNpcScript::mafl_is_3},
	{"mafl_is_4", &CNpcScript::mafl_is_4},
	{"mafl_is_5", &CNpcScript::mafl_is_5},
	{"mafl_is_6", &CNpcScript::mafl_is_6},
	{"mafl_is_7", &CNpcScript::mafl_is_7},
	{"mafl_is_8", &CNpcScript::mafl_is_8},
	{"mafl_is_9", &CNpcScript::mafl_is_9},
	{"mafl_is_26", &CNpcScript::mafl_is_26},
	{"mafl_is_42", &CNpcScript::mafl_is_42},
	{"mafl_is_43", &CNpcScript::mafl_is_43},
	{"mafl_is_10", &CNpcScript::mafl_is_10},
	{"mafl_hairshop_0", &CNpcScript::mafl_hairshop_0},
	{"mafl_hairshop_1", &CNpcScript::mafl_hairshop_1},
	{"mafl_hairshop_2", &CNpcScript::mafl_hairshop_2},
	{"mafl_hairshop_3", &CNpcScript::mafl_hairshop_3},
	{"mafl_hairshop_4", &CNpcScript::mafl_hairshop_4},
	{"mafl_hairshop_5", &CNpcScript::mafl_hairshop_5},
	{"mafl_hairshop_6", &CNpcScript::mafl_hairshop_6},
	{"mafl_hairshop_7", &CNpcScript::mafl_hairshop_7},
	{"mafl_hairshop_8", &CNpcScript::mafl_hairshop_8},
	{"mafl_hairshop_9", &CNpcScript::mafl_hairshop_9},
	{"mafl_hairshop_10", &CNpcScript::mafl_hairshop_10},
	{"mafl_ismeralda_0", &CNpcScript::mafl_ismeralda_0},
	{"mafl_ismeralda_1", &CNpcScript::mafl_ismeralda_1},
	{"mafl_ismeralda_2", &CNpcScript::mafl_ismeralda_2},
	{"mafl_ismeralda_3", &CNpcScript::mafl_ismeralda_3},
	{"mafl_ismeralda_4", &CNpcScript::mafl_ismeralda_4},
	{"mafl_ismeralda_5", &CNpcScript::mafl_ismeralda_5},
	{"mafl_ismeralda_6", &CNpcScript::mafl_ismeralda_6},
	{"mafl_ismeralda_7", &CNpcScript::mafl_ismeralda_7},
	{"mafl_ismeralda_8", &CNpcScript::mafl_ismeralda_8},
	{"mafl_ismeralda_9", &CNpcScript::mafl_ismeralda_9},
	{"mafl_ismeralda_26", &CNpcScript::mafl_ismeralda_26},
	{"mafl_ismeralda_42", &CNpcScript::mafl_ismeralda_42},
	{"mafl_ismeralda_43", &CNpcScript::mafl_ismeralda_43},
	{"mafl_ismeralda_10", &CNpcScript::mafl_ismeralda_10},
	{"mafl_ispim_0", &CNpcScript::mafl_ispim_0},
	{"mafl_ispim_1", &CNpcScript::mafl_ispim_1},
	{"mafl_ispim_2", &CNpcScript::mafl_ispim_2},
	{"mafl_ispim_3", &CNpcScript::mafl_ispim_3},
	{"mafl_ispim_4", &CNpcScript::mafl_ispim_4},
	{"mafl_ispim_5", &CNpcScript::mafl_ispim_5},
	{"mafl_ispim_6", &CNpcScript::mafl_ispim_6},
	{"mafl_ispim_7", &CNpcScript::mafl_ispim_7},
	{"mafl_ispim_8", &CNpcScript::mafl_ispim_8},
	{"mafl_ispim_9", &CNpcScript::mafl_ispim_9},
	{"mafl_ispim_26", &CNpcScript::mafl_ispim_26},
	{"mafl_ispim_42", &CNpcScript::mafl_ispim_42},
	{"mafl_ispim_43", &CNpcScript::mafl_ispim_43},
	{"mafl_ispim_10", &CNpcScript::mafl_ispim_10},
	{"mafl_isruel_0", &CNpcScript::mafl_isruel_0},
	{"mafl_isruel_1", &CNpcScript::mafl_isruel_1},
	{"mafl_isruel_2", &CNpcScript::mafl_isruel_2},
	{"mafl_isruel_3", &CNpcScript::mafl_isruel_3},
	{"mafl_isruel_4", &CNpcScript::mafl_isruel_4},
	{"mafl_isruel_5", &CNpcScript::mafl_isruel_5},
	{"mafl_isruel_6", &CNpcScript::mafl_isruel_6},
	{"mafl_isruel_7", &CNpcScript::mafl_isruel_7},
	{"mafl_isruel_8", &CNpcScript::mafl_isruel_8},
	{"mafl_isruel_9", &CNpcScript::mafl_isruel_9},
	{"mafl_isruel_26", &CNpcScript::mafl_isruel_26},
	{"mafl_isruel_42", &CNpcScript::mafl_isruel_42},
	{"mafl_isruel_43", &CNpcScript::mafl_isruel_43},
	{"mafl_isruel_10", &CNpcScript::mafl_isruel_10},
	{"mafl_jeff_0", &CNpcScript::mafl_jeff_0},
	{"mafl_jeff_1", &CNpcScript::mafl_jeff_1},
	{"mafl_jeff_2", &CNpcScript::mafl_jeff_2},
	{"mafl_jeff_3", &CNpcScript::mafl_jeff_3},
	{"mafl_jeff_4", &CNpcScript::mafl_jeff_4},
	{"mafl_jeff_5", &CNpcScript::mafl_jeff_5},
	{"mafl_jeff_6", &CNpcScript::mafl_jeff_6},
	{"mafl_jeff_8", &CNpcScript::mafl_jeff_8},
	{"mafl_jeff_9", &CNpcScript::mafl_jeff_9},
	{"mafl_jeff_10", &CNpcScript::mafl_jeff_10},
	{"mafl_juglin_0", &CNpcScript::mafl_juglin_0},
	{"mafl_juglin_1", &CNpcScript::mafl_juglin_1},
	{"mafl_juglin_2", &CNpcScript::mafl_juglin_2},
	{"mafl_juglin_3", &CNpcScript::mafl_juglin_3},
	{"mafl_juglin_4", &CNpcScript::mafl_juglin_4},
	{"mafl_juglin_5", &CNpcScript::mafl_juglin_5},
	{"mafl_juglin_6", &CNpcScript::mafl_juglin_6},
	{"mafl_juglin_8", &CNpcScript::mafl_juglin_8},
	{"mafl_juglin_9", &CNpcScript::mafl_juglin_9},
	{"mafl_juglin_10", &CNpcScript::mafl_juglin_10},
	{"mafl_juria_0", &CNpcScript::mafl_juria_0},
	{"mafl_juria_1", &CNpcScript::mafl_juria_1},
	{"mafl_juria_2", &CNpcScript::mafl_juria_2},
	{"mafl_juria_3", &CNpcScript::mafl_juria_3},
	{"mafl_juria_4", &CNpcScript::mafl_juria_4},
	{"mafl_juria_5", &CNpcScript::mafl_juria_5},
	{"mafl_juria_6", &CNpcScript::mafl_juria_6},
	{"mafl_juria_8", &CNpcScript::mafl_juria_8},
	{"mafl_juria_9", &CNpcScript::mafl_juria_9},
	{"mafl_juria_37", &CNpcScript::mafl_juria_37},
	{"mafl_juria_10", &CNpcScript::mafl_juria_10},
	{"mafl_kanane_0", &CNpcScript::mafl_kanane_0},
	{"mafl_kanane_1", &CNpcScript::mafl_kanane_1},
	{"mafl_kanane_2", &CNpcScript::mafl_kanane_2},
	{"mafl_kanane_3", &CNpcScript::mafl_kanane_3},
	{"mafl_kanane_4", &CNpcScript::mafl_kanane_4},
	{"mafl_kanane_5", &CNpcScript::mafl_kanane_5},
	{"mafl_kanane_6", &CNpcScript::mafl_kanane_6},
	{"mafl_kanane_8", &CNpcScript::mafl_kanane_8},
	{"mafl_kanane_9", &CNpcScript::mafl_kanane_9},
	{"mafl_kanane_10", &CNpcScript::mafl_kanane_10},
	{"mafl_kidmen_0", &CNpcScript::mafl_kidmen_0},
	{"mafl_kidmen_1", &CNpcScript::mafl_kidmen_1},
	{"mafl_kidmen_2", &CNpcScript::mafl_kidmen_2},
	{"mafl_kidmen_3", &CNpcScript::mafl_kidmen_3},
	{"mafl_kidmen_4", &CNpcScript::mafl_kidmen_4},
	{"mafl_kidmen_5", &CNpcScript::mafl_kidmen_5},
	{"mafl_kidmen_6", &CNpcScript::mafl_kidmen_6},
	{"mafl_kidmen_8", &CNpcScript::mafl_kidmen_8},
	{"mafl_kidmen_9", &CNpcScript::mafl_kidmen_9},
	{"mafl_kidmen_10", &CNpcScript::mafl_kidmen_10},
	{"mafl_kimel_0", &CNpcScript::mafl_kimel_0},
	{"mafl_kimel_1", &CNpcScript::mafl_kimel_1},
	{"mafl_kimel_2", &CNpcScript::mafl_kimel_2},
	{"mafl_kimel_3", &CNpcScript::mafl_kimel_3},
	{"mafl_kimel_4", &CNpcScript::mafl_kimel_4},
	{"mafl_kimel_5", &CNpcScript::mafl_kimel_5},
	{"mafl_kimel_6", &CNpcScript::mafl_kimel_6},
	{"mafl_kimel_7", &CNpcScript::mafl_kimel_7},
	{"mafl_kimel_8", &CNpcScript::mafl_kimel_8},
	{"mafl_kimel_9", &CNpcScript::mafl_kimel_9},
	{"mafl_kimel_10", &CNpcScript::mafl_kimel_10},
	{"mafl_kurumin_0", &CNpcScript::mafl_kurumin_0},
	{"mafl_kurumin_1", &CNpcScript::mafl_kurumin_1},
	{"mafl_kurumin_2", &CNpcScript::mafl_kurumin_2},
	{"mafl_kurumin_3", &CNpcScript::mafl_kurumin_3},
	{"mafl_kurumin_4", &CNpcScript::mafl_kurumin_4},
	{"mafl_kurumin_5", &CNpcScript::mafl_kurumin_5},
	{"mafl_kurumin_6", &CNpcScript::mafl_kurumin_6},
	{"mafl_kurumin_8", &CNpcScript::mafl_kurumin_8},
	{"mafl_kurumin_9", &CNpcScript::mafl_kurumin_9},
	{"mafl_kurumin_10", &CNpcScript::mafl_kurumin_10},
	{"mafl_langdrong_0", &CNpcScript::mafl_langdrong_0},
	{"mafl_langdrong_1", &CNpcScript::mafl_langdrong_1},
	{"mafl_langdrong_2", &CNpcScript::mafl_langdrong_2},
	{"mafl_langdrong_3", &CNpcScript::mafl_langdrong_3},
	{"mafl_langdrong_4", &CNpcScript::mafl_langdrong_4},
	{"mafl_langdrong_5", &CNpcScript::mafl_langdrong_5},
	{"mafl_langdrong_6", &CNpcScript::mafl_langdrong_6},
	{"mafl_langdrong_8", &CNpcScript::mafl_langdrong_8},
	{"mafl_langdrong_9", &CNpcScript::mafl_langdrong_9},
	{"mafl_langdrong_10", &CNpcScript::mafl_langdrong_10},
	{"mafl_lodelight_0", &CNpcScript::mafl_lodelight_0},
	{"mafl_lodelight_1", &CNpcScript::mafl_lodelight_1},
	{"mafl_lodelight_2", &CNpcScript::mafl_lodelight_2},
	{"mafl_lodelight_3", &CNpcScript::mafl_lodelight_3},
	{"mafl_lodelight_4", &CNpcScript::mafl_lodelight_4},
	{"mafl_lodelight_5", &CNpcScript::mafl_lodelight_5},
	{"mafl_lodelight_6", &CNpcScript::mafl_lodelight_6},
	{"mafl_lodelight_8", &CNpcScript::mafl_lodelight_8},
	{"mafl_lodelight_9", &CNpcScript::mafl_lodelight_9},
	{"mafl_lodelight_36", &CNpcScript::mafl_lodelight_36},
	{"mafl_lodelight_10", &CNpcScript::mafl_lodelight_10},
	{"mafl_losha_0", &CNpcScript::mafl_losha_0},
	{"mafl_losha_1", &CNpcScript::mafl_losha_1},
	{"mafl_losha_2", &CNpcScript::mafl_losha_2},
	{"mafl_losha_3", &CNpcScript::mafl_losha_3},
	{"mafl_losha_4", &CNpcScript::mafl_losha_4},
	{"mafl_losha_5", &CNpcScript::mafl_losha_5},
	{"mafl_losha_6", &CNpcScript::mafl_losha_6},
	{"mafl_losha_8", &CNpcScript::mafl_losha_8},
	{"mafl_losha_9", &CNpcScript::mafl_losha_9},
	{"mafl_losha_10", &CNpcScript::mafl_losha_10},
	{"mafl_loyah_0", &CNpcScript::mafl_loyah_0},
	{"mafl_loyah_1", &CNpcScript::mafl_loyah_1},
	{"mafl_loyah_2", &CNpcScript::mafl_loyah_2},
	{"mafl_loyah_3", &CNpcScript::mafl_loyah_3},
	{"mafl_loyah_4", &CNpcScript::mafl_loyah_4},
	{"mafl_loyah_5", &CNpcScript::mafl_loyah_5},
	{"mafl_loyah_6", &CNpcScript::mafl_loyah_6},
	{"mafl_loyah_8", &CNpcScript::mafl_loyah_8},
	{"mafl_loyah_9", &CNpcScript::mafl_loyah_9},
	{"mafl_loyah_10", &CNpcScript::mafl_loyah_10},
	{"mafl_luda_0", &CNpcScript::mafl_luda_0},
	{"mafl_luda_1", &CNpcScript::mafl_luda_1},
	{"mafl_luda_2", &CNpcScript::mafl_luda_2},
	{"mafl_luda_3", &CNpcScript::mafl_luda_3},
	{"mafl_luda_4", &CNpcScript::mafl_luda_4},
	{"mafl_luda_5", &CNpcScript::mafl_luda_5},
	{"mafl_luda_6", &CNpcScript::mafl_luda_6},
	{"mafl_luda_7", &CNpcScript::mafl_luda_7},
	{"mafl_luda_8", &CNpcScript::mafl_luda_8},
	{"mafl_luda_9", &CNpcScript::mafl_luda_9},
	{"mafl_luda_10", &CNpcScript::mafl_luda_10},
	{"mafl_lui_0", &CNpcScript::mafl_lui_0},
	{"mafl_lui_1", &CNpcScript::mafl_lui_1},
	{"mafl_lui_2", &CNpcScript::mafl_lui_2},
	{"mafl_lui_3", &CNpcScript::mafl_lui_3},
	{"mafl_lui_4", &CNpcScript::mafl_lui_4},
	{"mafl_lui_5", &CNpcScript::mafl_lui_5},
	{"mafl_lui_6", &CNpcScript::mafl_lui_6},
	{"mafl_lui_8", &CNpcScript::mafl_lui_8},
	{"mafl_lui_9", &CNpcScript::mafl_lui_9},
	{"mafl_lui_10", &CNpcScript::mafl_lui_10},
	{"mafl_maki_0", &CNpcScript::mafl_maki_0},
	{"mafl_maki_1", &CNpcScript::mafl_maki_1},
	{"mafl_maki_2", &CNpcScript::mafl_maki_2},
	{"mafl_maki_3", &CNpcScript::mafl_maki_3},
	{"mafl_maki_4", &CNpcScript::mafl_maki_4},
	{"mafl_maki_5", &CNpcScript::mafl_maki_5},
	{"mafl_maki_6", &CNpcScript::mafl_maki_6},
	{"mafl_maki_8", &CNpcScript::mafl_maki_8},
	{"mafl_maki_9", &CNpcScript::mafl_maki_9},
	{"mafl_maki_10", &CNpcScript::mafl_maki_10},
	{"mafl_marche_0", &CNpcScript::mafl_marche_0},
	{"mafl_marche_1", &CNpcScript::mafl_marche_1},
	{"mafl_marche_2", &CNpcScript::mafl_marche_2},
	{"mafl_marche_3", &CNpcScript::mafl_marche_3},
	{"mafl_marche_4", &CNpcScript::mafl_marche_4},
	{"mafl_marche_5", &CNpcScript::mafl_marche_5},
	{"mafl_marche_6", &CNpcScript::mafl_marche_6},
	{"mafl_marche_8", &CNpcScript::mafl_marche_8},
	{"mafl_marche_9", &CNpcScript::mafl_marche_9},
	{"mafl_marche_10", &CNpcScript::mafl_marche_10},
	{"mafl_martinyc_0", &CNpcScript::mafl_martinyc_0},
	{"mafl_martinyc_1", &CNpcScript::mafl_martinyc_1},
	{"mafl_martinyc_2", &CNpcScript::mafl_martinyc_2},
	{"mafl_martinyc_3", &CNpcScript::mafl_martinyc_3},
	{"mafl_martinyc_4", &CNpcScript::mafl_martinyc_4},
	{"mafl_martinyc_5", &CNpcScript::mafl_martinyc_5},
	{"mafl_martinyc_6", &CNpcScript::mafl_martinyc_6},
	{"mafl_martinyc_8", &CNpcScript::mafl_martinyc_8},
	{"mafl_martinyc_9", &CNpcScript::mafl_martinyc_9},
	{"mafl_martinyc_10", &CNpcScript::mafl_martinyc_10},
	{"mafl_mikyel_0", &CNpcScript::mafl_mikyel_0},
	{"mafl_mikyel_1", &CNpcScript::mafl_mikyel_1},
	{"mafl_mikyel_2", &CNpcScript::mafl_mikyel_2},
	{"mafl_mikyel_3", &CNpcScript::mafl_mikyel_3},
	{"mafl_mikyel_4", &CNpcScript::mafl_mikyel_4},
	{"mafl_mikyel_5", &CNpcScript::mafl_mikyel_5},
	{"mafl_mikyel_6", &CNpcScript::mafl_mikyel_6},
	{"mafl_mikyel_7", &CNpcScript::mafl_mikyel_7},
	{"mafl_mikyel_8", &CNpcScript::mafl_mikyel_8},
	{"mafl_mikyel_9", &CNpcScript::mafl_mikyel_9},
	{"mafl_mikyel_10", &CNpcScript::mafl_mikyel_10},
	{"mafl_mustang_0", &CNpcScript::mafl_mustang_0},
	{"mafl_mustang_1", &CNpcScript::mafl_mustang_1},
	{"mafl_mustang_2", &CNpcScript::mafl_mustang_2},
	{"mafl_mustang_3", &CNpcScript::mafl_mustang_3},
	{"mafl_mustang_4", &CNpcScript::mafl_mustang_4},
	{"mafl_mustang_5", &CNpcScript::mafl_mustang_5},
	{"mafl_mustang_6", &CNpcScript::mafl_mustang_6},
	{"mafl_mustang_8", &CNpcScript::mafl_mustang_8},
	{"mafl_mustang_9", &CNpcScript::mafl_mustang_9},
	{"mafl_mustang_10", &CNpcScript::mafl_mustang_10},
	{"mafl_nina_0", &CNpcScript::mafl_nina_0},
	{"mafl_nina_1", &CNpcScript::mafl_nina_1},
	{"mafl_nina_2", &CNpcScript::mafl_nina_2},
	{"mafl_nina_3", &CNpcScript::mafl_nina_3},
	{"mafl_nina_4", &CNpcScript::mafl_nina_4},
	{"mafl_nina_5", &CNpcScript::mafl_nina_5},
	{"mafl_nina_6", &CNpcScript::mafl_nina_6},
	{"mafl_nina_8", &CNpcScript::mafl_nina_8},
	{"mafl_nina_9", &CNpcScript::mafl_nina_9},
	{"mafl_nina_10", &CNpcScript::mafl_nina_10},
	{"mafl_noier_0", &CNpcScript::mafl_noier_0},
	{"mafl_noier_1", &CNpcScript::mafl_noier_1},
	{"mafl_noier_2", &CNpcScript::mafl_noier_2},
	{"mafl_noier_3", &CNpcScript::mafl_noier_3},
	{"mafl_noier_4", &CNpcScript::mafl_noier_4},
	{"mafl_noier_5", &CNpcScript::mafl_noier_5},
	{"mafl_noier_6", &CNpcScript::mafl_noier_6},
	{"mafl_noier_8", &CNpcScript::mafl_noier_8},
	{"mafl_noier_9", &CNpcScript::mafl_noier_9},
	{"mafl_noier_10", &CNpcScript::mafl_noier_10},
	{"mafl_official_0", &CNpcScript::mafl_official_0},
	{"mafl_official_1", &CNpcScript::mafl_official_1},
	{"mafl_official_2", &CNpcScript::mafl_official_2},
	{"mafl_official_3", &CNpcScript::mafl_official_3},
	{"mafl_official_4", &CNpcScript::mafl_official_4},
	{"mafl_official_5", &CNpcScript::mafl_official_5},
	{"mafl_official_6", &CNpcScript::mafl_official_6},
	{"mafl_official_8", &CNpcScript::mafl_official_8},
	{"mafl_official_9", &CNpcScript::mafl_official_9},
	{"mafl_official_10", &CNpcScript::mafl_official_10},
	{"mafl_phoho_0", &CNpcScript::mafl_phoho_0},
	{"mafl_phoho_1", &CNpcScript::mafl_phoho_1},
	{"mafl_phoho_2", &CNpcScript::mafl_phoho_2},
	{"mafl_phoho_3", &CNpcScript::mafl_phoho_3},
	{"mafl_phoho_4", &CNpcScript::mafl_phoho_4},
	{"mafl_phoho_5", &CNpcScript::mafl_phoho_5},
	{"mafl_phoho_6", &CNpcScript::mafl_phoho_6},
	{"mafl_phoho_8", &CNpcScript::mafl_phoho_8},
	{"mafl_phoho_9", &CNpcScript::mafl_phoho_9},
	{"mafl_phoho_10", &CNpcScript::mafl_phoho_10},
	{"mafl_pipi_0", &CNpcScript::mafl_pipi_0},
	{"mafl_pipi_1", &CNpcScript::mafl_pipi_1},
	{"mafl_pipi_2", &CNpcScript::mafl_pipi_2},
	{"mafl_pipi_3", &CNpcScript::mafl_pipi_3},
	{"mafl_pipi_4", &CNpcScript::mafl_pipi_4},
	{"mafl_pipi_5", &CNpcScript::mafl_pipi_5},
	{"mafl_pipi_6", &CNpcScript::mafl_pipi_6},
	{"mafl_pipi_8", &CNpcScript::mafl_pipi_8},
	{"mafl_pipi_9", &CNpcScript::mafl_pipi_9},
	{"mafl_pipi_10", &CNpcScript::mafl_pipi_10},
	{"mafl_radyon_0", &CNpcScript::mafl_radyon_0},
	{"mafl_radyon_1", &CNpcScript::mafl_radyon_1},
	{"mafl_radyon_2", &CNpcScript::mafl_radyon_2},
	{"mafl_radyon_3", &CNpcScript::mafl_radyon_3},
	{"mafl_radyon_4", &CNpcScript::mafl_radyon_4},
	{"mafl_radyon_5", &CNpcScript::mafl_radyon_5},
	{"mafl_radyon_6", &CNpcScript::mafl_radyon_6},
	{"mafl_radyon_7", &CNpcScript::mafl_radyon_7},
	{"mafl_radyon_8", &CNpcScript::mafl_radyon_8},
	{"mafl_radyon_9", &CNpcScript::mafl_radyon_9},
	{"mafl_radyon_10", &CNpcScript::mafl_radyon_10},
	{"mafl_rudvihil_0", &CNpcScript::mafl_rudvihil_0},
	{"mafl_rudvihil_1", &CNpcScript::mafl_rudvihil_1},
	{"mafl_rudvihil_2", &CNpcScript::mafl_rudvihil_2},
	{"mafl_rudvihil_3", &CNpcScript::mafl_rudvihil_3},
	{"mafl_rudvihil_4", &CNpcScript::mafl_rudvihil_4},
	{"mafl_rudvihil_5", &CNpcScript::mafl_rudvihil_5},
	{"mafl_rudvihil_6", &CNpcScript::mafl_rudvihil_6},
	{"mafl_rudvihil_7", &CNpcScript::mafl_rudvihil_7},
	{"mafl_rudvihil_8", &CNpcScript::mafl_rudvihil_8},
	{"mafl_rudvihil_9", &CNpcScript::mafl_rudvihil_9},
	{"mafl_rudvihil_10", &CNpcScript::mafl_rudvihil_10},
	{"mafl_ryupang_0", &CNpcScript::mafl_ryupang_0},
	{"mafl_ryupang_1", &CNpcScript::mafl_ryupang_1},
	{"mafl_ryupang_2", &CNpcScript::mafl_ryupang_2},
	{"mafl_ryupang_3", &CNpcScript::mafl_ryupang_3},
	{"mafl_ryupang_4", &CNpcScript::mafl_ryupang_4},
	{"mafl_ryupang_5", &CNpcScript::mafl_ryupang_5},
	{"mafl_ryupang_6", &CNpcScript::mafl_ryupang_6},
	{"mafl_ryupang_8", &CNpcScript::mafl_ryupang_8},
	{"mafl_ryupang_9", &CNpcScript::mafl_ryupang_9},
	{"mafl_ryupang_10", &CNpcScript::mafl_ryupang_10},
	{"mafl_segho_0", &CNpcScript::mafl_segho_0},
	{"mafl_segho_1", &CNpcScript::mafl_segho_1},
	{"mafl_segho_2", &CNpcScript::mafl_segho_2},
	{"mafl_segho_3", &CNpcScript::mafl_segho_3},
	{"mafl_segho_4", &CNpcScript::mafl_segho_4},
	{"mafl_segho_5", &CNpcScript::mafl_segho_5},
	{"mafl_segho_6", &CNpcScript::mafl_segho_6},
	{"mafl_segho_8", &CNpcScript::mafl_segho_8},
	{"mafl_segho_9", &CNpcScript::mafl_segho_9},
	{"mafl_segho_10", &CNpcScript::mafl_segho_10},
	{"mafl_sgradion_0", &CNpcScript::mafl_sgradion_0},
	{"mafl_sgradion_1", &CNpcScript::mafl_sgradion_1},
	{"mafl_sgradion_2", &CNpcScript::mafl_sgradion_2},
	{"mafl_sgradion_3", &CNpcScript::mafl_sgradion_3},
	{"mafl_sgradion_4", &CNpcScript::mafl_sgradion_4},
	{"mafl_sgradion_5", &CNpcScript::mafl_sgradion_5},
	{"mafl_sgradion_6", &CNpcScript::mafl_sgradion_6},
	{"mafl_sgradion_8", &CNpcScript::mafl_sgradion_8},
	{"mafl_sgradion_9", &CNpcScript::mafl_sgradion_9},
	{"mafl_sgradion_10", &CNpcScript::mafl_sgradion_10},
	{"mafl_ssotta_0", &CNpcScript::mafl_ssotta_0},
	{"mafl_ssotta_1", &CNpcScript::mafl_ssotta_1},
	{"mafl_ssotta_2", &CNpcScript::mafl_ssotta_2},
	{"mafl_ssotta_3", &CNpcScript::mafl_ssotta_3},
	{"mafl_ssotta_4", &CNpcScript::mafl_ssotta_4},
	{"mafl_ssotta_5", &CNpcScript::mafl_ssotta_5},
	{"mafl_ssotta_6", &CNpcScript::mafl_ssotta_6},
	{"mafl_ssotta_8", &CNpcScript::mafl_ssotta_8},
	{"mafl_ssotta_9", &CNpcScript::mafl_ssotta_9},
	{"mafl_ssotta_10", &CNpcScript::mafl_ssotta_10},
	{"mafl_teshar_0", &CNpcScript::mafl_teshar_0},
	{"mafl_teshar_1", &CNpcScript::mafl_teshar_1},
	{"mafl_teshar_2", &CNpcScript::mafl_teshar_2},
	{"mafl_teshar_3", &CNpcScript::mafl_teshar_3},
	{"mafl_teshar_4", &CNpcScript::mafl_teshar_4},
	{"mafl_teshar_5", &CNpcScript::mafl_teshar_5},
	{"mafl_teshar_6", &CNpcScript::mafl_teshar_6},
	{"mafl_teshar_8", &CNpcScript::mafl_teshar_8},
	{"mafl_teshar_9", &CNpcScript::mafl_teshar_9},
	{"mafl_teshar_10", &CNpcScript::mafl_teshar_10},
	{"mafl_tomba_0", &CNpcScript::mafl_tomba_0},
	{"mafl_tomba_1", &CNpcScript::mafl_tomba_1},
	{"mafl_tomba_2", &CNpcScript::mafl_tomba_2},
	{"mafl_tomba_3", &CNpcScript::mafl_tomba_3},
	{"mafl_tomba_4", &CNpcScript::mafl_tomba_4},
	{"mafl_tomba_5", &CNpcScript::mafl_tomba_5},
	{"mafl_tomba_6", &CNpcScript::mafl_tomba_6},
	{"mafl_tomba_8", &CNpcScript::mafl_tomba_8},
	{"mafl_tomba_9", &CNpcScript::mafl_tomba_9},
	{"mafl_tomba_10", &CNpcScript::mafl_tomba_10},
	{"mafl_tucani_0", &CNpcScript::mafl_tucani_0},
	{"mafl_tucani_1", &CNpcScript::mafl_tucani_1},
	{"mafl_tucani_2", &CNpcScript::mafl_tucani_2},
	{"mafl_tucani_3", &CNpcScript::mafl_tucani_3},
	{"mafl_tucani_4", &CNpcScript::mafl_tucani_4},
	{"mafl_tucani_5", &CNpcScript::mafl_tucani_5},
	{"mafl_tucani_6", &CNpcScript::mafl_tucani_6},
	{"mafl_tucani_7", &CNpcScript::mafl_tucani_7},
	{"mafl_tucani_8", &CNpcScript::mafl_tucani_8},
	{"mafl_tucani_9", &CNpcScript::mafl_tucani_9},
	{"mafl_tucani_10", &CNpcScript::mafl_tucani_10},
	{"mafl_ancimys_0", &CNpcScript::mafl_ancimys_0},
	{"mafl_ancimys_1", &CNpcScript::mafl_ancimys_1},
	{"mafl_ancimys_2", &CNpcScript::mafl_ancimys_2},
	{"mafl_ancimys_3", &CNpcScript::mafl_ancimys_3},
	{"mafl_ancimys_4", &CNpcScript::mafl_ancimys_4},
	{"mafl_ancimys_5", &CNpcScript::mafl_ancimys_5},
	{"mafl_ancimys_6", &CNpcScript::mafl_ancimys_6},
	{"mafl_ancimys_8", &CNpcScript::mafl_ancimys_8},
	{"mafl_ancimys_9", &CNpcScript::mafl_ancimys_9},
	{"mafl_ancimys_10", &CNpcScript::mafl_ancimys_10},
	{"mama_ancimys_0", &CNpcScript::mama_ancimys_0},
	{"mama_ancimys_1", &CNpcScript::mama_ancimys_1},
	{"mama_ancimys_2", &CNpcScript::mama_ancimys_2},
	{"mama_ancimys_3", &CNpcScript::mama_ancimys_3},
	{"mama_ancimys_4", &CNpcScript::mama_ancimys_4},
	{"mama_ancimys_5", &CNpcScript::mama_ancimys_5},
	{"mama_ancimys_6", &CNpcScript::mama_ancimys_6},
	{"mama_ancimys_8", &CNpcScript::mama_ancimys_8},
	{"mama_ancimys_9", &CNpcScript::mama_ancimys_9},
	{"mama_ancimys_10", &CNpcScript::mama_ancimys_10},
	{"masa_ann_0", &CNpcScript::masa_ann_0},
	{"masa_ann_1", &CNpcScript::masa_ann_1},
	{"masa_ann_2", &CNpcScript::masa_ann_2},
	{"masa_ann_3", &CNpcScript::masa_ann_3},
	{"masa_ann_4", &CNpcScript::masa_ann_4},
	{"masa_ann_5", &CNpcScript::masa_ann_5},
	{"masa_ann_6", &CNpcScript::masa_ann_6},
	{"masa_ann_8", &CNpcScript::masa_ann_8},
	{"masa_ann_9", &CNpcScript::masa_ann_9},
	{"masa_ann_26", &CNpcScript::masa_ann_26},
	{"masa_ann_42", &CNpcScript::masa_ann_42},
	{"masa_ann_43", &CNpcScript::masa_ann_43},
	{"masa_ann_10", &CNpcScript::masa_ann_10},
	{"masa_bill_0", &CNpcScript::masa_bill_0},
	{"masa_bill_1", &CNpcScript::masa_bill_1},
	{"masa_bill_2", &CNpcScript::masa_bill_2},
	{"masa_bill_3", &CNpcScript::masa_bill_3},
	{"masa_bill_4", &CNpcScript::masa_bill_4},
	{"masa_bill_5", &CNpcScript::masa_bill_5},
	{"masa_bill_6", &CNpcScript::masa_bill_6},
	{"masa_bill_8", &CNpcScript::masa_bill_8},
	{"masa_bill_9", &CNpcScript::masa_bill_9},
	{"masa_bill_10", &CNpcScript::masa_bill_10},
	{"masa_bowler_0", &CNpcScript::masa_bowler_0},
	{"masa_bowler_1", &CNpcScript::masa_bowler_1},
	{"masa_bowler_2", &CNpcScript::masa_bowler_2},
	{"masa_bowler_3", &CNpcScript::masa_bowler_3},
	{"masa_bowler_4", &CNpcScript::masa_bowler_4},
	{"masa_bowler_5", &CNpcScript::masa_bowler_5},
	{"masa_bowler_6", &CNpcScript::masa_bowler_6},
	{"masa_bowler_7", &CNpcScript::masa_bowler_7},
	{"masa_bowler_8", &CNpcScript::masa_bowler_8},
	{"masa_bowler_9", &CNpcScript::masa_bowler_9},
	{"masa_bowler_10", &CNpcScript::masa_bowler_10},
	{"masa_bozman_0", &CNpcScript::masa_bozman_0},
	{"masa_bozman_1", &CNpcScript::masa_bozman_1},
	{"masa_bozman_2", &CNpcScript::masa_bozman_2},
	{"masa_bozman_3", &CNpcScript::masa_bozman_3},
	{"masa_bozman_4", &CNpcScript::masa_bozman_4},
	{"masa_bozman_5", &CNpcScript::masa_bozman_5},
	{"masa_bozman_6", &CNpcScript::masa_bozman_6},
	{"masa_bozman_8", &CNpcScript::masa_bozman_8},
	{"masa_bozman_9", &CNpcScript::masa_bozman_9},
	{"masa_bozman_10", &CNpcScript::masa_bozman_10},
	{"masa_brodeay_0", &CNpcScript::masa_brodeay_0},
	{"masa_brodeay_1", &CNpcScript::masa_brodeay_1},
	{"masa_brodeay_2", &CNpcScript::masa_brodeay_2},
	{"masa_brodeay_3", &CNpcScript::masa_brodeay_3},
	{"masa_brodeay_4", &CNpcScript::masa_brodeay_4},
	{"masa_brodeay_5", &CNpcScript::masa_brodeay_5},
	{"masa_brodeay_6", &CNpcScript::masa_brodeay_6},
	{"masa_brodeay_8", &CNpcScript::masa_brodeay_8},
	{"masa_brodeay_9", &CNpcScript::masa_brodeay_9},
	{"masa_brodeay_10", &CNpcScript::masa_brodeay_10},
	{"masa_bulrox_0", &CNpcScript::masa_bulrox_0},
	{"masa_bulrox_1", &CNpcScript::masa_bulrox_1},
	{"masa_bulrox_2", &CNpcScript::masa_bulrox_2},
	{"masa_bulrox_3", &CNpcScript::masa_bulrox_3},
	{"masa_bulrox_4", &CNpcScript::masa_bulrox_4},
	{"masa_bulrox_5", &CNpcScript::masa_bulrox_5},
	{"masa_bulrox_6", &CNpcScript::masa_bulrox_6},
	{"masa_bulrox_8", &CNpcScript::masa_bulrox_8},
	{"masa_bulrox_9", &CNpcScript::masa_bulrox_9},
	{"masa_bulrox_10", &CNpcScript::masa_bulrox_10},
	{"masa_colack_0", &CNpcScript::masa_colack_0},
	{"masa_colack_1", &CNpcScript::masa_colack_1},
	{"masa_colack_2", &CNpcScript::masa_colack_2},
	{"masa_colack_3", &CNpcScript::masa_colack_3},
	{"masa_colack_4", &CNpcScript::masa_colack_4},
	{"masa_colack_5", &CNpcScript::masa_colack_5},
	{"masa_colack_6", &CNpcScript::masa_colack_6},
	{"masa_colack_7", &CNpcScript::masa_colack_7},
	{"masa_colack_8", &CNpcScript::masa_colack_8},
	{"masa_colack_9", &CNpcScript::masa_colack_9},
	{"masa_colack_10", &CNpcScript::masa_colack_10},
	{"masa_drestern_0", &CNpcScript::masa_drestern_0},
	{"masa_drestern_1", &CNpcScript::masa_drestern_1},
	{"masa_drestern_2", &CNpcScript::masa_drestern_2},
	{"masa_drestern_3", &CNpcScript::masa_drestern_3},
	{"masa_drestern_4", &CNpcScript::masa_drestern_4},
	{"masa_drestern_5", &CNpcScript::masa_drestern_5},
	{"masa_drestern_6", &CNpcScript::masa_drestern_6},
	{"masa_drestern_8", &CNpcScript::masa_drestern_8},
	{"masa_drestern_9", &CNpcScript::masa_drestern_9},
	{"masa_drestern_10", &CNpcScript::masa_drestern_10},
	{"masa_furan_0", &CNpcScript::masa_furan_0},
	{"masa_furan_1", &CNpcScript::masa_furan_1},
	{"masa_furan_2", &CNpcScript::masa_furan_2},
	{"masa_furan_3", &CNpcScript::masa_furan_3},
	{"masa_furan_4", &CNpcScript::masa_furan_4},
	{"masa_furan_5", &CNpcScript::masa_furan_5},
	{"masa_furan_6", &CNpcScript::masa_furan_6},
	{"masa_furan_7", &CNpcScript::masa_furan_7},
	{"masa_furan_8", &CNpcScript::masa_furan_8},
	{"masa_furan_9", &CNpcScript::masa_furan_9},
	{"masa_furan_10", &CNpcScript::masa_furan_10},
	{"masa_gothante_0", &CNpcScript::masa_gothante_0},
	{"masa_gothante_1", &CNpcScript::masa_gothante_1},
	{"masa_gothante_2", &CNpcScript::masa_gothante_2},
	{"masa_gothante_3", &CNpcScript::masa_gothante_3},
	{"masa_gothante_4", &CNpcScript::masa_gothante_4},
	{"masa_gothante_5", &CNpcScript::masa_gothante_5},
	{"masa_gothante_6", &CNpcScript::masa_gothante_6},
	{"masa_gothante_8", &CNpcScript::masa_gothante_8},
	{"masa_gothante_9", &CNpcScript::masa_gothante_9},
	{"masa_gothante_10", &CNpcScript::masa_gothante_10},
	{"masa_hee_0", &CNpcScript::masa_hee_0},
	{"masa_hee_1", &CNpcScript::masa_hee_1},
	{"masa_hee_2", &CNpcScript::masa_hee_2},
	{"masa_hee_3", &CNpcScript::masa_hee_3},
	{"masa_hee_4", &CNpcScript::masa_hee_4},
	{"masa_hee_5", &CNpcScript::masa_hee_5},
	{"masa_hee_6", &CNpcScript::masa_hee_6},
	{"masa_hee_8", &CNpcScript::masa_hee_8},
	{"masa_hee_9", &CNpcScript::masa_hee_9},
	{"masa_hee_10", &CNpcScript::masa_hee_10},
	{"masa_helena_0", &CNpcScript::masa_helena_0},
	{"masa_helena_1", &CNpcScript::masa_helena_1},
	{"masa_helena_2", &CNpcScript::masa_helena_2},
	{"masa_helena_3", &CNpcScript::masa_helena_3},
	{"masa_helena_4", &CNpcScript::masa_helena_4},
	{"masa_helena_5", &CNpcScript::masa_helena_5},
	{"masa_helena_6", &CNpcScript::masa_helena_6},
	{"masa_helena_8", &CNpcScript::masa_helena_8},
	{"masa_helena_9", &CNpcScript::masa_helena_9},
	{"masa_helena_10", &CNpcScript::masa_helena_10},
	{"masa_helena_865", &CNpcScript::masa_helena_865},
	{"masa_helena_866", &CNpcScript::masa_helena_866},
	{"masa_helgar_0", &CNpcScript::masa_helgar_0},
	{"masa_helgar_1", &CNpcScript::masa_helgar_1},
	{"masa_helgar_2", &CNpcScript::masa_helgar_2},
	{"masa_helgar_3", &CNpcScript::masa_helgar_3},
	{"masa_helgar_4", &CNpcScript::masa_helgar_4},
	{"masa_helgar_5", &CNpcScript::masa_helgar_5},
	{"masa_helgar_6", &CNpcScript::masa_helgar_6},
	{"masa_helgar_7", &CNpcScript::masa_helgar_7},
	{"masa_helgar_8", &CNpcScript::masa_helgar_8},
	{"masa_helgar_9", &CNpcScript::masa_helgar_9},
	{"masa_helgar_10", &CNpcScript::masa_helgar_10},
	{"masa_heltung_0", &CNpcScript::masa_heltung_0},
	{"masa_heltung_1", &CNpcScript::masa_heltung_1},
	{"masa_heltung_2", &CNpcScript::masa_heltung_2},
	{"masa_heltung_3", &CNpcScript::masa_heltung_3},
	{"masa_heltung_4", &CNpcScript::masa_heltung_4},
	{"masa_heltung_5", &CNpcScript::masa_heltung_5},
	{"masa_heltung_6", &CNpcScript::masa_heltung_6},
	{"masa_heltung_7", &CNpcScript::masa_heltung_7},
	{"masa_heltung_8", &CNpcScript::masa_heltung_8},
	{"masa_heltung_9", &CNpcScript::masa_heltung_9},
	{"masa_heltung_10", &CNpcScript::masa_heltung_10},
	{"masa_herth_0", &CNpcScript::masa_herth_0},
	{"masa_herth_1", &CNpcScript::masa_herth_1},
	{"masa_herth_2", &CNpcScript::masa_herth_2},
	{"masa_herth_3", &CNpcScript::masa_herth_3},
	{"masa_herth_4", &CNpcScript::masa_herth_4},
	{"masa_herth_5", &CNpcScript::masa_herth_5},
	{"masa_herth_6", &CNpcScript::masa_herth_6},
	{"masa_herth_8", &CNpcScript::masa_herth_8},
	{"masa_herth_9", &CNpcScript::masa_herth_9},
	{"masa_herth_26", &CNpcScript::masa_herth_26},
	{"masa_herth_42", &CNpcScript::masa_herth_42},
	{"masa_herth_43", &CNpcScript::masa_herth_43},
	{"masa_herth_10", &CNpcScript::masa_herth_10},
	{"masa_janne_0", &CNpcScript::masa_janne_0},
	{"masa_janne_1", &CNpcScript::masa_janne_1},
	{"masa_janne_2", &CNpcScript::masa_janne_2},
	{"masa_janne_3", &CNpcScript::masa_janne_3},
	{"masa_janne_4", &CNpcScript::masa_janne_4},
	{"masa_janne_5", &CNpcScript::masa_janne_5},
	{"masa_janne_6", &CNpcScript::masa_janne_6},
	{"masa_janne_8", &CNpcScript::masa_janne_8},
	{"masa_janne_9", &CNpcScript::masa_janne_9},
	{"masa_janne_26", &CNpcScript::masa_janne_26},
	{"masa_janne_42", &CNpcScript::masa_janne_42},
	{"masa_janne_43", &CNpcScript::masa_janne_43},
	{"masa_janne_10", &CNpcScript::masa_janne_10},
	{"masa_jeonghwa_0", &CNpcScript::masa_jeonghwa_0},
	{"masa_jeonghwa_1", &CNpcScript::masa_jeonghwa_1},
	{"masa_jeonghwa_2", &CNpcScript::masa_jeonghwa_2},
	{"masa_jeonghwa_3", &CNpcScript::masa_jeonghwa_3},
	{"masa_jeonghwa_4", &CNpcScript::masa_jeonghwa_4},
	{"masa_jeonghwa_5", &CNpcScript::masa_jeonghwa_5},
	{"masa_jeonghwa_6", &CNpcScript::masa_jeonghwa_6},
	{"masa_jeonghwa_8", &CNpcScript::masa_jeonghwa_8},
	{"masa_jeonghwa_9", &CNpcScript::masa_jeonghwa_9},
	{"masa_jeonghwa_10", &CNpcScript::masa_jeonghwa_10},
	{"masa_karin_0", &CNpcScript::masa_karin_0},
	{"masa_karin_1", &CNpcScript::masa_karin_1},
	{"masa_karin_2", &CNpcScript::masa_karin_2},
	{"masa_karin_3", &CNpcScript::masa_karin_3},
	{"masa_karin_4", &CNpcScript::masa_karin_4},
	{"masa_karin_5", &CNpcScript::masa_karin_5},
	{"masa_karin_6", &CNpcScript::masa_karin_6},
	{"masa_karin_8", &CNpcScript::masa_karin_8},
	{"masa_karin_9", &CNpcScript::masa_karin_9},
	{"masa_karin_10", &CNpcScript::masa_karin_10},
	{"masa_kimberley_0", &CNpcScript::masa_kimberley_0},
	{"masa_kimberley_1", &CNpcScript::masa_kimberley_1},
	{"masa_kimberley_2", &CNpcScript::masa_kimberley_2},
	{"masa_kimberley_3", &CNpcScript::masa_kimberley_3},
	{"masa_kimberley_4", &CNpcScript::masa_kimberley_4},
	{"masa_kimberley_5", &CNpcScript::masa_kimberley_5},
	{"masa_kimberley_6", &CNpcScript::masa_kimberley_6},
	{"masa_kimberley_8", &CNpcScript::masa_kimberley_8},
	{"masa_kimberley_9", &CNpcScript::masa_kimberley_9},
	{"masa_kimberley_26", &CNpcScript::masa_kimberley_26},
	{"masa_kimberley_42", &CNpcScript::masa_kimberley_42},
	{"masa_kimberley_43", &CNpcScript::masa_kimberley_43},
	{"masa_kimberley_10", &CNpcScript::masa_kimberley_10},
	{"masa_lancomi_0", &CNpcScript::masa_lancomi_0},
	{"masa_lancomi_1", &CNpcScript::masa_lancomi_1},
	{"masa_lancomi_2", &CNpcScript::masa_lancomi_2},
	{"masa_lancomi_3", &CNpcScript::masa_lancomi_3},
	{"masa_lancomi_4", &CNpcScript::masa_lancomi_4},
	{"masa_lancomi_5", &CNpcScript::masa_lancomi_5},
	{"masa_lancomi_6", &CNpcScript::masa_lancomi_6},
	{"masa_lancomi_7", &CNpcScript::masa_lancomi_7},
	{"masa_lancomi_8", &CNpcScript::masa_lancomi_8},
	{"masa_lancomi_9", &CNpcScript::masa_lancomi_9},
	{"masa_lancomi_10", &CNpcScript::masa_lancomi_10},
	{"masa_leann_0", &CNpcScript::masa_leann_0},
	{"masa_leann_1", &CNpcScript::masa_leann_1},
	{"masa_leann_2", &CNpcScript::masa_leann_2},
	{"masa_leann_3", &CNpcScript::masa_leann_3},
	{"masa_leann_4", &CNpcScript::masa_leann_4},
	{"masa_leann_5", &CNpcScript::masa_leann_5},
	{"masa_leann_6", &CNpcScript::masa_leann_6},
	{"masa_leann_8", &CNpcScript::masa_leann_8},
	{"masa_leann_9", &CNpcScript::masa_leann_9},
	{"masa_leann_26", &CNpcScript::masa_leann_26},
	{"masa_leann_42", &CNpcScript::masa_leann_42},
	{"masa_leann_43", &CNpcScript::masa_leann_43},
	{"masa_leann_10", &CNpcScript::masa_leann_10},
	{"masa_lodelight_0", &CNpcScript::masa_lodelight_0},
	{"masa_lodelight_1", &CNpcScript::masa_lodelight_1},
	{"masa_lodelight_2", &CNpcScript::masa_lodelight_2},
	{"masa_lodelight_3", &CNpcScript::masa_lodelight_3},
	{"masa_lodelight_4", &CNpcScript::masa_lodelight_4},
	{"masa_lodelight_5", &CNpcScript::masa_lodelight_5},
	{"masa_lodelight_6", &CNpcScript::masa_lodelight_6},
	{"masa_lodelight_8", &CNpcScript::masa_lodelight_8},
	{"masa_lodelight_9", &CNpcScript::masa_lodelight_9},
	{"masa_lodelight_36", &CNpcScript::masa_lodelight_36},
	{"masa_lodelight_10", &CNpcScript::masa_lodelight_10},
	{"masa_lopaze_0", &CNpcScript::masa_lopaze_0},
	{"masa_lopaze_1", &CNpcScript::masa_lopaze_1},
	{"masa_lopaze_2", &CNpcScript::masa_lopaze_2},
	{"masa_lopaze_3", &CNpcScript::masa_lopaze_3},
	{"masa_lopaze_4", &CNpcScript::masa_lopaze_4},
	{"masa_lopaze_5", &CNpcScript::masa_lopaze_5},
	{"masa_lopaze_6", &CNpcScript::masa_lopaze_6},
	{"masa_lopaze_8", &CNpcScript::masa_lopaze_8},
	{"masa_lopaze_9", &CNpcScript::masa_lopaze_9},
	{"masa_lopaze_10", &CNpcScript::masa_lopaze_10},
	{"masa_martin_0", &CNpcScript::masa_martin_0},
	{"masa_martin_1", &CNpcScript::masa_martin_1},
	{"masa_martin_2", &CNpcScript::masa_martin_2},
	{"masa_martin_3", &CNpcScript::masa_martin_3},
	{"masa_martin_4", &CNpcScript::masa_martin_4},
	{"masa_martin_5", &CNpcScript::masa_martin_5},
	{"masa_martin_6", &CNpcScript::masa_martin_6},
	{"masa_martin_8", &CNpcScript::masa_martin_8},
	{"masa_martin_9", &CNpcScript::masa_martin_9},
	{"masa_martin_10", &CNpcScript::masa_martin_10},
	{"masa_mayun_0", &CNpcScript::masa_mayun_0},
	{"masa_mayun_1", &CNpcScript::masa_mayun_1},
	{"masa_mayun_2", &CNpcScript::masa_mayun_2},
	{"masa_mayun_3", &CNpcScript::masa_mayun_3},
	{"masa_mayun_4", &CNpcScript::masa_mayun_4},
	{"masa_mayun_5", &CNpcScript::masa_mayun_5},
	{"masa_mayun_6", &CNpcScript::masa_mayun_6},
	{"masa_mayun_8", &CNpcScript::masa_mayun_8},
	{"masa_mayun_9", &CNpcScript::masa_mayun_9},
	{"masa_mayun_10", &CNpcScript::masa_mayun_10},
	{"masa_parine_0", &CNpcScript::masa_parine_0},
	{"masa_parine_1", &CNpcScript::masa_parine_1},
	{"masa_parine_2", &CNpcScript::masa_parine_2},
	{"masa_parine_3", &CNpcScript::masa_parine_3},
	{"masa_parine_4", &CNpcScript::masa_parine_4},
	{"masa_parine_5", &CNpcScript::masa_parine_5},
	{"masa_parine_6", &CNpcScript::masa_parine_6},
	{"masa_parine_8", &CNpcScript::masa_parine_8},
	{"masa_parine_9", &CNpcScript::masa_parine_9},
	{"masa_parine_10", &CNpcScript::masa_parine_10},
	{"masa_porgo_0", &CNpcScript::masa_porgo_0},
	{"masa_porgo_1", &CNpcScript::masa_porgo_1},
	{"masa_porgo_2", &CNpcScript::masa_porgo_2},
	{"masa_porgo_3", &CNpcScript::masa_porgo_3},
	{"masa_porgo_4", &CNpcScript::masa_porgo_4},
	{"masa_porgo_5", &CNpcScript::masa_porgo_5},
	{"masa_porgo_6", &CNpcScript::masa_porgo_6},
	{"masa_porgo_8", &CNpcScript::masa_porgo_8},
	{"masa_porgo_9", &CNpcScript::masa_porgo_9},
	{"masa_porgo_10", &CNpcScript::masa_porgo_10},
	{"masa_queercollector_0", &CNpcScript::masa_queercollector_0},
	{"masa_queercollector_1", &CNpcScript::masa_queercollector_1},
	{"masa_queercollector_2", &CNpcScript::masa_queercollector_2},
	{"masa_queercollector_3", &CNpcScript::masa_queercollector_3},
	{"masa_queercollector_4", &CNpcScript::masa_queercollector_4},
	{"masa_queercollector_5", &CNpcScript::masa_queercollector_5},
	{"masa_queercollector_6", &CNpcScript::masa_queercollector_6},
	{"masa_queercollector_7", &CNpcScript::masa_queercollector_7},
	{"masa_queercollector_8", &CNpcScript::masa_queercollector_8},
	{"masa_queercollector_9", &CNpcScript::masa_queercollector_9},
	{"masa_queercollector_10", &CNpcScript::masa_queercollector_10},
	{"masa_rovanett_0", &CNpcScript::masa_rovanett_0},
	{"masa_rovanett_1", &CNpcScript::masa_rovanett_1},
	{"masa_rovanett_2", &CNpcScript::masa_rovanett_2},
	{"masa_rovanett_3", &CNpcScript::masa_rovanett_3},
	{"masa_rovanett_4", &CNpcScript::masa_rovanett_4},
	{"masa_rovanett_5", &CNpcScript::masa_rovanett_5},
	{"masa_rovanett_6", &CNpcScript::masa_rovanett_6},
	{"masa_rovanett_8", &CNpcScript::masa_rovanett_8},
	{"masa_rovanett_9", &CNpcScript::masa_rovanett_9},
	{"masa_rovanett_10", &CNpcScript::masa_rovanett_10},
	{"masa_sainmayor_0", &CNpcScript::masa_sainmayor_0},
	{"masa_sainmayor_1", &CNpcScript::masa_sainmayor_1},
	{"masa_sainmayor_2", &CNpcScript::masa_sainmayor_2},
	{"masa_sainmayor_3", &CNpcScript::masa_sainmayor_3},
	{"masa_sainmayor_4", &CNpcScript::masa_sainmayor_4},
	{"masa_sainmayor_5", &CNpcScript::masa_sainmayor_5},
	{"masa_sainmayor_6", &CNpcScript::masa_sainmayor_6},
	{"masa_sainmayor_8", &CNpcScript::masa_sainmayor_8},
	{"masa_sainmayor_9", &CNpcScript::masa_sainmayor_9},
	{"masa_sainmayor_10", &CNpcScript::masa_sainmayor_10},
	{"masa_tina_0", &CNpcScript::masa_tina_0},
	{"masa_tina_1", &CNpcScript::masa_tina_1},
	{"masa_tina_2", &CNpcScript::masa_tina_2},
	{"masa_tina_3", &CNpcScript::masa_tina_3},
	{"masa_tina_4", &CNpcScript::masa_tina_4},
	{"masa_tina_5", &CNpcScript::masa_tina_5},
	{"masa_tina_6", &CNpcScript::masa_tina_6},
	{"masa_tina_8", &CNpcScript::masa_tina_8},
	{"masa_tina_9", &CNpcScript::masa_tina_9},
	{"masa_tina_40", &CNpcScript::masa_tina_40},
	{"masa_tina_41", &CNpcScript::masa_tina_41},
	{"masa_tina_10", &CNpcScript::masa_tina_10},
	{"masa_troupemember1_0", &CNpcScript::masa_troupemember1_0},
	{"masa_troupemember1_1", &CNpcScript::masa_troupemember1_1},
	{"masa_troupemember1_2", &CNpcScript::masa_troupemember1_2},
	{"masa_troupemember1_3", &CNpcScript::masa_troupemember1_3},
	{"masa_troupemember1_4", &CNpcScript::masa_troupemember1_4},
	{"masa_troupemember1_5", &CNpcScript::masa_troupemember1_5},
	{"masa_troupemember1_6", &CNpcScript::masa_troupemember1_6},
	{"masa_troupemember1_7", &CNpcScript::masa_troupemember1_7},
	{"masa_troupemember1_8", &CNpcScript::masa_troupemember1_8},
	{"masa_troupemember1_9", &CNpcScript::masa_troupemember1_9},
	{"masa_troupemember1_10", &CNpcScript::masa_troupemember1_10},
	{"masa_troupemember2_0", &CNpcScript::masa_troupemember2_0},
	{"masa_troupemember2_1", &CNpcScript::masa_troupemember2_1},
	{"masa_troupemember2_2", &CNpcScript::masa_troupemember2_2},
	{"masa_troupemember2_3", &CNpcScript::masa_troupemember2_3},
	{"masa_troupemember2_4", &CNpcScript::masa_troupemember2_4},
	{"masa_troupemember2_5", &CNpcScript::masa_troupemember2_5},
	{"masa_troupemember2_6", &CNpcScript::masa_troupemember2_6},
	{"masa_troupemember2_7", &CNpcScript::masa_troupemember2_7},
	{"masa_troupemember2_8", &CNpcScript::masa_troupemember2_8},
	{"masa_troupemember2_9", &CNpcScript::masa_troupemember2_9},
	{"masa_troupemember2_10", &CNpcScript::masa_troupemember2_10},
	{"masa_troupemember3_0", &CNpcScript::masa_troupemember3_0},
	{"masa_troupemember3_1", &CNpcScript::masa_troupemember3_1},
	{"masa_troupemember3_2", &CNpcScript::masa_troupemember3_2},
	{"masa_troupemember3_3", &CNpcScript::masa_troupemember3_3},
	{"masa_troupemember3_4", &CNpcScript::masa_troupemember3_4},
	{"masa_troupemember3_5", &CNpcScript::masa_troupemember3_5},
	{"masa_troupemember3_6", &CNpcScript::masa_troupemember3_6},
	{"masa_troupemember3_7", &CNpcScript::masa_troupemember3_7},
	{"masa_troupemember3_8", &CNpcScript::masa_troupemember3_8},
	{"masa_troupemember3_9", &CNpcScript::masa_troupemember3_9},
	{"masa_troupemember3_10", &CNpcScript::masa_troupemember3_10},
	{"masa_troupemember4_0", &CNpcScript::masa_troupemember4_0},
	{"masa_troupemember4_1", &CNpcScript::masa_troupemember4_1},
	{"masa_troupemember4_2", &CNpcScript::masa_troupemember4_2},
	{"masa_troupemember4_3", &CNpcScript::masa_troupemember4_3},
	{"masa_troupemember4_4", &CNpcScript::masa_troupemember4_4},
	{"masa_troupemember4_5", &CNpcScript::masa_troupemember4_5},
	{"masa_troupemember4_6", &CNpcScript::masa_troupemember4_6},
	{"masa_troupemember4_7", &CNpcScript::masa_troupemember4_7},
	{"masa_troupemember4_8", &CNpcScript::masa_troupemember4_8},
	{"masa_troupemember4_9", &CNpcScript::masa_troupemember4_9},
	{"masa_troupemember4_10", &CNpcScript::masa_troupemember4_10},
	{"masa_troupemember5_0", &CNpcScript::masa_troupemember5_0},
	{"masa_troupemember5_1", &CNpcScript::masa_troupemember5_1},
	{"masa_troupemember5_2", &CNpcScript::masa_troupemember5_2},
	{"masa_troupemember5_3", &CNpcScript::masa_troupemember5_3},
	{"masa_troupemember5_4", &CNpcScript::masa_troupemember5_4},
	{"masa_troupemember5_5", &CNpcScript::masa_troupemember5_5},
	{"masa_troupemember5_6", &CNpcScript::masa_troupemember5_6},
	{"masa_troupemember5_7", &CNpcScript::masa_troupemember5_7},
	{"masa_troupemember5_8", &CNpcScript::masa_troupemember5_8},
	{"masa_troupemember5_9", &CNpcScript::masa_troupemember5_9},
	{"masa_troupemember5_10", &CNpcScript::masa_troupemember5_10},
	{"masa_wingyei_0", &CNpcScript::masa_wingyei_0},
	{"masa_wingyei_1", &CNpcScript::masa_wingyei_1},
	{"masa_wingyei_2", &CNpcScript::masa_wingyei_2},
	{"masa_wingyei_3", &CNpcScript::masa_wingyei_3},
	{"masa_wingyei_4", &CNpcScript::masa_wingyei_4},
	{"masa_wingyei_5", &CNpcScript::masa_wingyei_5},
	{"masa_wingyei_6", &CNpcScript::masa_wingyei_6},
	{"masa_wingyei_8", &CNpcScript::masa_wingyei_8},
	{"masa_wingyei_9", &CNpcScript::masa_wingyei_9},
	{"masa_wingyei_10", &CNpcScript::masa_wingyei_10},
	{"mada_darmayor_0", &CNpcScript::mada_darmayor_0},
	{"mada_darmayor_1", &CNpcScript::mada_darmayor_1},
	{"mada_darmayor_2", &CNpcScript::mada_darmayor_2},
	{"mada_darmayor_3", &CNpcScript::mada_darmayor_3},
	{"mada_darmayor_4", &CNpcScript::mada_darmayor_4},
	{"mada_darmayor_5", &CNpcScript::mada_darmayor_5},
	{"mada_darmayor_6", &CNpcScript::mada_darmayor_6},
	{"mada_darmayor_8", &CNpcScript::mada_darmayor_8},
	{"mada_darmayor_9", &CNpcScript::mada_darmayor_9},
	{"mada_darmayor_10", &CNpcScript::mada_darmayor_10},
	{"masa_daz_0", &CNpcScript::masa_daz_0},
	{"masa_daz_1", &CNpcScript::masa_daz_1},
	{"masa_daz_2", &CNpcScript::masa_daz_2},
	{"masa_daz_3", &CNpcScript::masa_daz_3},
	{"masa_daz_4", &CNpcScript::masa_daz_4},
	{"masa_daz_5", &CNpcScript::masa_daz_5},
	{"masa_daz_6", &CNpcScript::masa_daz_6},
	{"masa_daz_8", &CNpcScript::masa_daz_8},
	{"masa_daz_9", &CNpcScript::masa_daz_9},
	{"masa_daz_10", &CNpcScript::masa_daz_10},
	{"mafl_iblis01_0", &CNpcScript::mafl_iblis01_0},
	{"mafl_iblis01_1", &CNpcScript::mafl_iblis01_1},
	{"mafl_iblis01_2", &CNpcScript::mafl_iblis01_2},
	{"mafl_iblis01_3", &CNpcScript::mafl_iblis01_3},
	{"mafl_iblis01_4", &CNpcScript::mafl_iblis01_4},
	{"mafl_iblis01_5", &CNpcScript::mafl_iblis01_5},
	{"mafl_iblis01_6", &CNpcScript::mafl_iblis01_6},
	{"mafl_iblis01_8", &CNpcScript::mafl_iblis01_8},
	{"mafl_iblis01_9", &CNpcScript::mafl_iblis01_9},
	{"mafl_iblis01_10", &CNpcScript::mafl_iblis01_10},
	{"mafl_iblis02_0", &CNpcScript::mafl_iblis02_0},
	{"mafl_iblis02_1", &CNpcScript::mafl_iblis02_1},
	{"mafl_iblis02_2", &CNpcScript::mafl_iblis02_2},
	{"mafl_iblis02_3", &CNpcScript::mafl_iblis02_3},
	{"mafl_iblis02_4", &CNpcScript::mafl_iblis02_4},
	{"mafl_iblis02_5", &CNpcScript::mafl_iblis02_5},
	{"mafl_iblis02_6", &CNpcScript::mafl_iblis02_6},
	{"mafl_iblis02_8", &CNpcScript::mafl_iblis02_8},
	{"mafl_iblis02_9", &CNpcScript::mafl_iblis02_9},
	{"mafl_iblis02_10", &CNpcScript::mafl_iblis02_10},
	{"mafl_iblis03_0", &CNpcScript::mafl_iblis03_0},
	{"mafl_iblis03_1", &CNpcScript::mafl_iblis03_1},
	{"mafl_iblis03_2", &CNpcScript::mafl_iblis03_2},
	{"mafl_iblis03_3", &CNpcScript::mafl_iblis03_3},
	{"mafl_iblis03_4", &CNpcScript::mafl_iblis03_4},
	{"mafl_iblis03_5", &CNpcScript::mafl_iblis03_5},
	{"mafl_iblis03_6", &CNpcScript::mafl_iblis03_6},
	{"mafl_iblis03_8", &CNpcScript::mafl_iblis03_8},
	{"mafl_iblis03_9", &CNpcScript::mafl_iblis03_9},
	{"mafl_iblis03_10", &CNpcScript::mafl_iblis03_10},
	{"mafl_iblis04_0", &CNpcScript::mafl_iblis04_0},
	{"mafl_iblis04_1", &CNpcScript::mafl_iblis04_1},
	{"mafl_iblis04_2", &CNpcScript::mafl_iblis04_2},
	{"mafl_iblis04_3", &CNpcScript::mafl_iblis04_3},
	{"mafl_iblis04_4", &CNpcScript::mafl_iblis04_4},
	{"mafl_iblis04_5", &CNpcScript::mafl_iblis04_5},
	{"mafl_iblis04_6", &CNpcScript::mafl_iblis04_6},
	{"mafl_iblis04_8", &CNpcScript::mafl_iblis04_8},
	{"mafl_iblis04_9", &CNpcScript::mafl_iblis04_9},
	{"mafl_iblis04_10", &CNpcScript::mafl_iblis04_10},
	{"mafl_iblis05_0", &CNpcScript::mafl_iblis05_0},
	{"mafl_iblis05_1", &CNpcScript::mafl_iblis05_1},
	{"mafl_iblis05_2", &CNpcScript::mafl_iblis05_2},
	{"mafl_iblis05_3", &CNpcScript::mafl_iblis05_3},
	{"mafl_iblis05_4", &CNpcScript::mafl_iblis05_4},
	{"mafl_iblis05_5", &CNpcScript::mafl_iblis05_5},
	{"mafl_iblis05_6", &CNpcScript::mafl_iblis05_6},
	{"mafl_iblis05_8", &CNpcScript::mafl_iblis05_8},
	{"mafl_iblis05_9", &CNpcScript::mafl_iblis05_9},
	{"mafl_iblis05_10", &CNpcScript::mafl_iblis05_10},
	{"mafl_autumnevent01_0", &CNpcScript::mafl_autumnevent01_0},
	{"mafl_autumnevent01_1", &CNpcScript::mafl_autumnevent01_1},
	{"mafl_autumnevent01_2", &CNpcScript::mafl_autumnevent01_2},
	{"mafl_autumnevent01_3", &CNpcScript::mafl_autumnevent01_3},
	{"mafl_autumnevent01_4", &CNpcScript::mafl_autumnevent01_4},
	{"mafl_autumnevent01_5", &CNpcScript::mafl_autumnevent01_5},
	{"mafl_autumnevent01_6", &CNpcScript::mafl_autumnevent01_6},
	{"mafl_autumnevent01_8", &CNpcScript::mafl_autumnevent01_8},
	{"mafl_autumnevent01_9", &CNpcScript::mafl_autumnevent01_9},
	{"mafl_autumnevent01_10", &CNpcScript::mafl_autumnevent01_10},
	{"mafl_autumnevent02_0", &CNpcScript::mafl_autumnevent02_0},
	{"mafl_autumnevent02_1", &CNpcScript::mafl_autumnevent02_1},
	{"mafl_autumnevent02_2", &CNpcScript::mafl_autumnevent02_2},
	{"mafl_autumnevent02_3", &CNpcScript::mafl_autumnevent02_3},
	{"mafl_autumnevent02_4", &CNpcScript::mafl_autumnevent02_4},
	{"mafl_autumnevent02_5", &CNpcScript::mafl_autumnevent02_5},
	{"mafl_autumnevent02_6", &CNpcScript::mafl_autumnevent02_6},
	{"mafl_autumnevent02_8", &CNpcScript::mafl_autumnevent02_8},
	{"mafl_autumnevent02_9", &CNpcScript::mafl_autumnevent02_9},
	{"mafl_autumnevent02_10", &CNpcScript::mafl_autumnevent02_10},
	{"mafl_autumnevent03_0", &CNpcScript::mafl_autumnevent03_0},
	{"mafl_autumnevent03_1", &CNpcScript::mafl_autumnevent03_1},
	{"mafl_autumnevent03_2", &CNpcScript::mafl_autumnevent03_2},
	{"mafl_autumnevent03_3", &CNpcScript::mafl_autumnevent03_3},
	{"mafl_autumnevent03_4", &CNpcScript::mafl_autumnevent03_4},
	{"mafl_autumnevent03_5", &CNpcScript::mafl_autumnevent03_5},
	{"mafl_autumnevent03_6", &CNpcScript::mafl_autumnevent03_6},
	{"mafl_autumnevent03_8", &CNpcScript::mafl_autumnevent03_8},
	{"mafl_autumnevent03_9", &CNpcScript::mafl_autumnevent03_9},
	{"mafl_autumnevent03_10", &CNpcScript::mafl_autumnevent03_10},
	{"mafl_autumnevent04_0", &CNpcScript::mafl_autumnevent04_0},
	{"mafl_autumnevent04_1", &CNpcScript::mafl_autumnevent04_1},
	{"mafl_autumnevent04_2", &CNpcScript::mafl_autumnevent04_2},
	{"mafl_autumnevent04_3", &CNpcScript::mafl_autumnevent04_3},
	{"mafl_autumnevent04_4", &CNpcScript::mafl_autumnevent04_4},
	{"mafl_autumnevent04_5", &CNpcScript::mafl_autumnevent04_5},
	{"mafl_autumnevent04_6", &CNpcScript::mafl_autumnevent04_6},
	{"mafl_autumnevent04_8", &CNpcScript::mafl_autumnevent04_8},
	{"mafl_autumnevent04_9", &CNpcScript::mafl_autumnevent04_9},
	{"mafl_autumnevent04_10", &CNpcScript::mafl_autumnevent04_10},
	{"mama_pknpc01_0", &CNpcScript::mama_pknpc01_0},
	{"mama_pknpc01_1", &CNpcScript::mama_pknpc01_1},
	{"mama_pknpc01_2", &CNpcScript::mama_pknpc01_2},
	{"mama_pknpc01_3", &CNpcScript::mama_pknpc01_3},
	{"mama_pknpc01_4", &CNpcScript::mama_pknpc01_4},
	{"mama_pknpc01_5", &CNpcScript::mama_pknpc01_5},
	{"mama_pknpc01_6", &CNpcScript::mama_pknpc01_6},
	{"mama_pknpc01_8", &CNpcScript::mama_pknpc01_8},
	{"mama_pknpc01_9", &CNpcScript::mama_pknpc01_9},
	{"mama_pknpc01_10", &CNpcScript::mama_pknpc01_10},
	{"mafl_teacher01_0", &CNpcScript::mafl_teacher01_0},
	{"mafl_teacher01_1", &CNpcScript::mafl_teacher01_1},
	{"mafl_teacher01_2", &CNpcScript::mafl_teacher01_2},
	{"mafl_teacher01_3", &CNpcScript::mafl_teacher01_3},
	{"mafl_teacher01_4", &CNpcScript::mafl_teacher01_4},
	{"mafl_teacher01_5", &CNpcScript::mafl_teacher01_5},
	{"mafl_teacher01_6", &CNpcScript::mafl_teacher01_6},
	{"mafl_teacher01_8", &CNpcScript::mafl_teacher01_8},
	{"mafl_teacher01_9", &CNpcScript::mafl_teacher01_9},
	{"mafl_teacher01_10", &CNpcScript::mafl_teacher01_10},
	{"mafl_teacher02_0", &CNpcScript::mafl_teacher02_0},
	{"mafl_teacher02_1", &CNpcScript::mafl_teacher02_1},
	{"mafl_teacher02_2", &CNpcScript::mafl_teacher02_2},
	{"mafl_teacher02_3", &CNpcScript::mafl_teacher02_3},
	{"mafl_teacher02_4", &CNpcScript::mafl_teacher02_4},
	{"mafl_teacher02_5", &CNpcScript::mafl_teacher02_5},
	{"mafl_teacher02_6", &CNpcScript::mafl_teacher02_6},
	{"mafl_teacher02_8", &CNpcScript::mafl_teacher02_8},
	{"mafl_teacher02_9", &CNpcScript::mafl_teacher02_9},
	{"mafl_teacher02_10", &CNpcScript::mafl_teacher02_10},
	{"mafl_teacher03_0", &CNpcScript::mafl_teacher03_0},
	{"mafl_teacher03_1", &CNpcScript::mafl_teacher03_1},
	{"mafl_teacher03_2", &CNpcScript::mafl_teacher03_2},
	{"mafl_teacher03_3", &CNpcScript::mafl_teacher03_3},
	{"mafl_teacher03_4", &CNpcScript::mafl_teacher03_4},
	{"mafl_teacher03_5", &CNpcScript::mafl_teacher03_5},
	{"mafl_teacher03_6", &CNpcScript::mafl_teacher03_6},
	{"mafl_teacher03_8", &CNpcScript::mafl_teacher03_8},
	{"mafl_teacher03_9", &CNpcScript::mafl_teacher03_9},
	{"mafl_teacher03_10", &CNpcScript::mafl_teacher03_10},
	{"mafl_cooker01_0", &CNpcScript::mafl_cooker01_0},
	{"mafl_cooker01_1", &CNpcScript::mafl_cooker01_1},
	{"mafl_cooker01_2", &CNpcScript::mafl_cooker01_2},
	{"mafl_cooker01_3", &CNpcScript::mafl_cooker01_3},
	{"mafl_cooker01_4", &CNpcScript::mafl_cooker01_4},
	{"mafl_cooker01_5", &CNpcScript::mafl_cooker01_5},
	{"mafl_cooker01_6", &CNpcScript::mafl_cooker01_6},
	{"mafl_cooker01_8", &CNpcScript::mafl_cooker01_8},
	{"mafl_cooker01_9", &CNpcScript::mafl_cooker01_9},
	{"mafl_cooker01_10", &CNpcScript::mafl_cooker01_10},
	{"mafl_cooker02_0", &CNpcScript::mafl_cooker02_0},
	{"mafl_cooker02_1", &CNpcScript::mafl_cooker02_1},
	{"mafl_cooker02_2", &CNpcScript::mafl_cooker02_2},
	{"mafl_cooker02_3", &CNpcScript::mafl_cooker02_3},
	{"mafl_cooker02_4", &CNpcScript::mafl_cooker02_4},
	{"mafl_cooker02_5", &CNpcScript::mafl_cooker02_5},
	{"mafl_cooker02_6", &CNpcScript::mafl_cooker02_6},
	{"mafl_cooker02_8", &CNpcScript::mafl_cooker02_8},
	{"mafl_cooker02_9", &CNpcScript::mafl_cooker02_9},
	{"mafl_cooker02_10", &CNpcScript::mafl_cooker02_10},
	{"mafl_arborday01_0", &CNpcScript::mafl_arborday01_0},
	{"mafl_arborday01_1", &CNpcScript::mafl_arborday01_1},
	{"mafl_arborday01_2", &CNpcScript::mafl_arborday01_2},
	{"mafl_arborday01_3", &CNpcScript::mafl_arborday01_3},
	{"mafl_arborday01_4", &CNpcScript::mafl_arborday01_4},
	{"mafl_arborday01_5", &CNpcScript::mafl_arborday01_5},
	{"mafl_arborday01_6", &CNpcScript::mafl_arborday01_6},
	{"mafl_arborday01_8", &CNpcScript::mafl_arborday01_8},
	{"mafl_arborday01_9", &CNpcScript::mafl_arborday01_9},
	{"mafl_arborday01_10", &CNpcScript::mafl_arborday01_10},
	{"mafl_arborday02_0", &CNpcScript::mafl_arborday02_0},
	{"mafl_arborday02_1", &CNpcScript::mafl_arborday02_1},
	{"mafl_arborday02_2", &CNpcScript::mafl_arborday02_2},
	{"mafl_arborday02_3", &CNpcScript::mafl_arborday02_3},
	{"mafl_arborday02_4", &CNpcScript::mafl_arborday02_4},
	{"mafl_arborday02_5", &CNpcScript::mafl_arborday02_5},
	{"mafl_arborday02_6", &CNpcScript::mafl_arborday02_6},
	{"mafl_arborday02_8", &CNpcScript::mafl_arborday02_8},
	{"mafl_arborday02_9", &CNpcScript::mafl_arborday02_9},
	{"mafl_arborday02_10", &CNpcScript::mafl_arborday02_10},
	{"mafl_arborday03_0", &CNpcScript::mafl_arborday03_0},
	{"mafl_arborday03_1", &CNpcScript::mafl_arborday03_1},
	{"mafl_arborday03_2", &CNpcScript::mafl_arborday03_2},
	{"mafl_arborday03_3", &CNpcScript::mafl_arborday03_3},
	{"mafl_arborday03_4", &CNpcScript::mafl_arborday03_4},
	{"mafl_arborday03_5", &CNpcScript::mafl_arborday03_5},
	{"mafl_arborday03_6", &CNpcScript::mafl_arborday03_6},
	{"mafl_arborday03_8", &CNpcScript::mafl_arborday03_8},
	{"mafl_arborday03_9", &CNpcScript::mafl_arborday03_9},
	{"mafl_arborday03_10", &CNpcScript::mafl_arborday03_10},
	{"mafl_arborday04_0", &CNpcScript::mafl_arborday04_0},
	{"mafl_arborday04_1", &CNpcScript::mafl_arborday04_1},
	{"mafl_arborday04_2", &CNpcScript::mafl_arborday04_2},
	{"mafl_arborday04_3", &CNpcScript::mafl_arborday04_3},
	{"mafl_arborday04_4", &CNpcScript::mafl_arborday04_4},
	{"mafl_arborday04_5", &CNpcScript::mafl_arborday04_5},
	{"mafl_arborday04_6", &CNpcScript::mafl_arborday04_6},
	{"mafl_arborday04_8", &CNpcScript::mafl_arborday04_8},
	{"mafl_arborday04_9", &CNpcScript::mafl_arborday04_9},
	{"mafl_arborday04_10", &CNpcScript::mafl_arborday04_10},
	{"mafl_pang_0", &CNpcScript::mafl_pang_0},
	{"mafl_pang_1", &CNpcScript::mafl_pang_1},
	{"mafl_pang_2", &CNpcScript::mafl_pang_2},
	{"mafl_pang_3", &CNpcScript::mafl_pang_3},
	{"mafl_pang_4", &CNpcScript::mafl_pang_4},
	{"mafl_pang_5", &CNpcScript::mafl_pang_5},
	{"mafl_pang_6", &CNpcScript::mafl_pang_6},
	{"mafl_pang_8", &CNpcScript::mafl_pang_8},
	{"mafl_pang_9", &CNpcScript::mafl_pang_9},
	{"mafl_pang_10", &CNpcScript::mafl_pang_10},
	{"mafl_kawibawibo01_0", &CNpcScript::mafl_kawibawibo01_0},
	{"mafl_kawibawibo01_1", &CNpcScript::mafl_kawibawibo01_1},
	{"mafl_kawibawibo01_2", &CNpcScript::mafl_kawibawibo01_2},
	{"mafl_kawibawibo01_3", &CNpcScript::mafl_kawibawibo01_3},
	{"mafl_kawibawibo01_4", &CNpcScript::mafl_kawibawibo01_4},
	{"mafl_kawibawibo01_5", &CNpcScript::mafl_kawibawibo01_5},
	{"mafl_kawibawibo01_6", &CNpcScript::mafl_kawibawibo01_6},
	{"mafl_kawibawibo01_8", &CNpcScript::mafl_kawibawibo01_8},
	{"mafl_kawibawibo01_9", &CNpcScript::mafl_kawibawibo01_9},
	{"mafl_kawibawibo01_10", &CNpcScript::mafl_kawibawibo01_10},
	{"mada_redrobeman_0", &CNpcScript::mada_redrobeman_0},
	{"mada_redrobeman_1", &CNpcScript::mada_redrobeman_1},
	{"mada_redrobeman_2", &CNpcScript::mada_redrobeman_2},
	{"mada_redrobeman_3", &CNpcScript::mada_redrobeman_3},
	{"mada_redrobeman_4", &CNpcScript::mada_redrobeman_4},
	{"mada_redrobeman_5", &CNpcScript::mada_redrobeman_5},
	{"mada_redrobeman_6", &CNpcScript::mada_redrobeman_6},
	{"mada_redrobeman_8", &CNpcScript::mada_redrobeman_8},
	{"mada_redrobeman_9", &CNpcScript::mada_redrobeman_9},
	{"mada_redrobeman_10", &CNpcScript::mada_redrobeman_10},
	{"mada_redrobegirl_0", &CNpcScript::mada_redrobegirl_0},
	{"mada_redrobegirl_1", &CNpcScript::mada_redrobegirl_1},
	{"mada_redrobegirl_2", &CNpcScript::mada_redrobegirl_2},
	{"mada_redrobegirl_3", &CNpcScript::mada_redrobegirl_3},
	{"mada_redrobegirl_4", &CNpcScript::mada_redrobegirl_4},
	{"mada_redrobegirl_5", &CNpcScript::mada_redrobegirl_5},
	{"mada_redrobegirl_6", &CNpcScript::mada_redrobegirl_6},
	{"mada_redrobegirl_8", &CNpcScript::mada_redrobegirl_8},
	{"mada_redrobegirl_9", &CNpcScript::mada_redrobegirl_9},
	{"mada_redrobegirl_10", &CNpcScript::mada_redrobegirl_10},
	{"mafl_pettamer_0", &CNpcScript::mafl_pettamer_0},
	{"mafl_pettamer_1", &CNpcScript::mafl_pettamer_1},
	{"mafl_pettamer_2", &CNpcScript::mafl_pettamer_2},
	{"mafl_pettamer_3", &CNpcScript::mafl_pettamer_3},
	{"mafl_pettamer_4", &CNpcScript::mafl_pettamer_4},
	{"mafl_pettamer_5", &CNpcScript::mafl_pettamer_5},
	{"mafl_pettamer_6", &CNpcScript::mafl_pettamer_6},
	{"mafl_pettamer_8", &CNpcScript::mafl_pettamer_8},
	{"mafl_pettamer_9", &CNpcScript::mafl_pettamer_9},
	{"mafl_pettamer_10", &CNpcScript::mafl_pettamer_10},
	{"mada_gatekeeper_0", &CNpcScript::mada_gatekeeper_0},
	{"mada_gatekeeper_1", &CNpcScript::mada_gatekeeper_1},
	{"mada_gatekeeper_2", &CNpcScript::mada_gatekeeper_2},
	{"mada_gatekeeper_3", &CNpcScript::mada_gatekeeper_3},
	{"mada_gatekeeper_4", &CNpcScript::mada_gatekeeper_4},
	{"mada_gatekeeper_5", &CNpcScript::mada_gatekeeper_5},
	{"mada_gatekeeper_6", &CNpcScript::mada_gatekeeper_6},
	{"mada_gatekeeper_8", &CNpcScript::mada_gatekeeper_8},
	{"mada_gatekeeper_9", &CNpcScript::mada_gatekeeper_9},
	{"mada_gatekeeper_10", &CNpcScript::mada_gatekeeper_10},
	{"mafl_july01_0", &CNpcScript::mafl_july01_0},
	{"mafl_july01_1", &CNpcScript::mafl_july01_1},
	{"mafl_july01_2", &CNpcScript::mafl_july01_2},
	{"mafl_july01_3", &CNpcScript::mafl_july01_3},
	{"mafl_july01_4", &CNpcScript::mafl_july01_4},
	{"mafl_july01_5", &CNpcScript::mafl_july01_5},
	{"mafl_july01_6", &CNpcScript::mafl_july01_6},
	{"mafl_july01_8", &CNpcScript::mafl_july01_8},
	{"mafl_july01_9", &CNpcScript::mafl_july01_9},
	{"mafl_july01_10", &CNpcScript::mafl_july01_10},
	{"mafl_july02_0", &CNpcScript::mafl_july02_0},
	{"mafl_july02_1", &CNpcScript::mafl_july02_1},
	{"mafl_july02_2", &CNpcScript::mafl_july02_2},
	{"mafl_july02_3", &CNpcScript::mafl_july02_3},
	{"mafl_july02_4", &CNpcScript::mafl_july02_4},
	{"mafl_july02_5", &CNpcScript::mafl_july02_5},
	{"mafl_july02_6", &CNpcScript::mafl_july02_6},
	{"mafl_july02_8", &CNpcScript::mafl_july02_8},
	{"mafl_july02_9", &CNpcScript::mafl_july02_9},
	{"mafl_july02_10", &CNpcScript::mafl_july02_10},
	{"mafl_bobochan_0", &CNpcScript::mafl_bobochan_0},
	{"mafl_bobochan_1", &CNpcScript::mafl_bobochan_1},
	{"mafl_bobochan_2", &CNpcScript::mafl_bobochan_2},
	{"mafl_bobochan_3", &CNpcScript::mafl_bobochan_3},
	{"mafl_bobochan_4", &CNpcScript::mafl_bobochan_4},
	{"mafl_bobochan_5", &CNpcScript::mafl_bobochan_5},
	{"mafl_bobochan_6", &CNpcScript::mafl_bobochan_6},
	{"mafl_bobochan_8", &CNpcScript::mafl_bobochan_8},
	{"mafl_bobochan_9", &CNpcScript::mafl_bobochan_9},
	{"mafl_bobochan_10", &CNpcScript::mafl_bobochan_10},
	{"mafl_nestle02_0", &CNpcScript::mafl_nestle02_0},
	{"mafl_nestle02_1", &CNpcScript::mafl_nestle02_1},
	{"mafl_nestle02_2", &CNpcScript::mafl_nestle02_2},
	{"mafl_nestle02_3", &CNpcScript::mafl_nestle02_3},
	{"mafl_nestle02_4", &CNpcScript::mafl_nestle02_4},
	{"mafl_nestle02_5", &CNpcScript::mafl_nestle02_5},
	{"mafl_nestle02_6", &CNpcScript::mafl_nestle02_6},
	{"mafl_nestle02_8", &CNpcScript::mafl_nestle02_8},
	{"mafl_nestle02_9", &CNpcScript::mafl_nestle02_9},
	{"mafl_nestle02_10", &CNpcScript::mafl_nestle02_10},
	{"mafl_nestle03_0", &CNpcScript::mafl_nestle03_0},
	{"mafl_nestle03_1", &CNpcScript::mafl_nestle03_1},
	{"mafl_nestle03_2", &CNpcScript::mafl_nestle03_2},
	{"mafl_nestle03_3", &CNpcScript::mafl_nestle03_3},
	{"mafl_nestle03_4", &CNpcScript::mafl_nestle03_4},
	{"mafl_nestle03_5", &CNpcScript::mafl_nestle03_5},
	{"mafl_nestle03_6", &CNpcScript::mafl_nestle03_6},
	{"mafl_nestle03_8", &CNpcScript::mafl_nestle03_8},
	{"mafl_nestle03_9", &CNpcScript::mafl_nestle03_9},
	{"mafl_nestle03_10", &CNpcScript::mafl_nestle03_10},
	{"mafl_kawibawibo03_0", &CNpcScript::mafl_kawibawibo03_0},
	{"mafl_kawibawibo03_1", &CNpcScript::mafl_kawibawibo03_1},
	{"mafl_kawibawibo03_2", &CNpcScript::mafl_kawibawibo03_2},
	{"mafl_kawibawibo03_3", &CNpcScript::mafl_kawibawibo03_3},
	{"mafl_kawibawibo03_4", &CNpcScript::mafl_kawibawibo03_4},
	{"mafl_kawibawibo03_5", &CNpcScript::mafl_kawibawibo03_5},
	{"mafl_kawibawibo03_6", &CNpcScript::mafl_kawibawibo03_6},
	{"mafl_kawibawibo03_8", &CNpcScript::mafl_kawibawibo03_8},
	{"mafl_kawibawibo03_9", &CNpcScript::mafl_kawibawibo03_9},
	{"mafl_kawibawibo03_10", &CNpcScript::mafl_kawibawibo03_10},
	{"mafl_strangecollector_0", &CNpcScript::mafl_strangecollector_0},
	{"mafl_strangecollector_1", &CNpcScript::mafl_strangecollector_1},
	{"mafl_strangecollector_2", &CNpcScript::mafl_strangecollector_2},
	{"mafl_strangecollector_3", &CNpcScript::mafl_strangecollector_3},
	{"mafl_strangecollector_4", &CNpcScript::mafl_strangecollector_4},
	{"mafl_strangecollector_5", &CNpcScript::mafl_strangecollector_5},
	{"mafl_strangecollector_6", &CNpcScript::mafl_strangecollector_6},
	{"mafl_strangecollector_8", &CNpcScript::mafl_strangecollector_8},
	{"mafl_strangecollector_9", &CNpcScript::mafl_strangecollector_9},
	{"mafl_strangecollector_10", &CNpcScript::mafl_strangecollector_10},
	{"mafl_amos_0", &CNpcScript::mafl_amos_0},
	{"mafl_amos_1", &CNpcScript::mafl_amos_1},
	{"mafl_amos_2", &CNpcScript::mafl_amos_2},
	{"mafl_amos_3", &CNpcScript::mafl_amos_3},
	{"mafl_amos_4", &CNpcScript::mafl_amos_4},
	{"mafl_amos_5", &CNpcScript::mafl_amos_5},
	{"mafl_amos_6", &CNpcScript::mafl_amos_6},
	{"mafl_amos_8", &CNpcScript::mafl_amos_8},
	{"mafl_amos_9", &CNpcScript::mafl_amos_9},
	{"mafl_amos_10", &CNpcScript::mafl_amos_10},
	{"mafl_annie_0", &CNpcScript::mafl_annie_0},
	{"mafl_annie_1", &CNpcScript::mafl_annie_1},
	{"mafl_annie_2", &CNpcScript::mafl_annie_2},
	{"mafl_annie_3", &CNpcScript::mafl_annie_3},
	{"mafl_annie_4", &CNpcScript::mafl_annie_4},
	{"mafl_annie_5", &CNpcScript::mafl_annie_5},
	{"mafl_annie_6", &CNpcScript::mafl_annie_6},
	{"mafl_annie_8", &CNpcScript::mafl_annie_8},
	{"mafl_annie_9", &CNpcScript::mafl_annie_9},
	{"mafl_annie_10", &CNpcScript::mafl_annie_10},
	{"mafl_waforu_0", &CNpcScript::mafl_waforu_0},
	{"mafl_waforu_1", &CNpcScript::mafl_waforu_1},
	{"mafl_waforu_2", &CNpcScript::mafl_waforu_2},
	{"mafl_waforu_3", &CNpcScript::mafl_waforu_3},
	{"mafl_waforu_4", &CNpcScript::mafl_waforu_4},
	{"mafl_waforu_5", &CNpcScript::mafl_waforu_5},
	{"mafl_waforu_6", &CNpcScript::mafl_waforu_6},
	{"mafl_waforu_8", &CNpcScript::mafl_waforu_8},
	{"mafl_waforu_9", &CNpcScript::mafl_waforu_9},
	{"mafl_waforu_10", &CNpcScript::mafl_waforu_10},
	{"mafl_santa01_0", &CNpcScript::mafl_santa01_0},
	{"mafl_santa01_1", &CNpcScript::mafl_santa01_1},
	{"mafl_santa01_2", &CNpcScript::mafl_santa01_2},
	{"mafl_santa01_3", &CNpcScript::mafl_santa01_3},
	{"mafl_santa01_4", &CNpcScript::mafl_santa01_4},
	{"mafl_santa01_5", &CNpcScript::mafl_santa01_5},
	{"mafl_santa01_6", &CNpcScript::mafl_santa01_6},
	{"mafl_santa01_8", &CNpcScript::mafl_santa01_8},
	{"mafl_santa01_9", &CNpcScript::mafl_santa01_9},
	{"mafl_santa01_10", &CNpcScript::mafl_santa01_10},
	{"mafl_harold_0", &CNpcScript::mafl_harold_0},
	{"mafl_harold_1", &CNpcScript::mafl_harold_1},
	{"mafl_harold_2", &CNpcScript::mafl_harold_2},
	{"mafl_harold_3", &CNpcScript::mafl_harold_3},
	{"mafl_harold_4", &CNpcScript::mafl_harold_4},
	{"mafl_harold_5", &CNpcScript::mafl_harold_5},
	{"mafl_harold_6", &CNpcScript::mafl_harold_6},
	{"mafl_harold_8", &CNpcScript::mafl_harold_8},
	{"mafl_harold_9", &CNpcScript::mafl_harold_9},
	{"mafl_harold_10", &CNpcScript::mafl_harold_10},
	{"mafl_dick_0", &CNpcScript::mafl_dick_0},
	{"mafl_dick_1", &CNpcScript::mafl_dick_1},
	{"mafl_dick_2", &CNpcScript::mafl_dick_2},
	{"mafl_dick_3", &CNpcScript::mafl_dick_3},
	{"mafl_dick_4", &CNpcScript::mafl_dick_4},
	{"mafl_dick_5", &CNpcScript::mafl_dick_5},
	{"mafl_dick_6", &CNpcScript::mafl_dick_6},
	{"mafl_dick_8", &CNpcScript::mafl_dick_8},
	{"mafl_dick_9", &CNpcScript::mafl_dick_9},
	{"mafl_dick_10", &CNpcScript::mafl_dick_10},
	{"mafl_helper_0", &CNpcScript::mafl_helper_0},
	{"mafl_helper_1", &CNpcScript::mafl_helper_1},
	{"mafl_helper_2", &CNpcScript::mafl_helper_2},
	{"mafl_helper_3", &CNpcScript::mafl_helper_3},
	{"mafl_helper_4", &CNpcScript::mafl_helper_4},
	{"mafl_helper_5", &CNpcScript::mafl_helper_5},
	{"mafl_helper_6", &CNpcScript::mafl_helper_6},
	{"mafl_helper_8", &CNpcScript::mafl_helper_8},
	{"mafl_helper_9", &CNpcScript::mafl_helper_9},
	{"mafl_helper_10", &CNpcScript::mafl_helper_10},
	{"mafl_ray_0", &CNpcScript::mafl_ray_0},
	{"mafl_ray_1", &CNpcScript::mafl_ray_1},
	{"mafl_ray_2", &CNpcScript::mafl_ray_2},
	{"mafl_ray_3", &CNpcScript::mafl_ray_3},
	{"mafl_ray_4", &CNpcScript::mafl_ray_4},
	{"mafl_ray_5", &CNpcScript::mafl_ray_5},
	{"mafl_ray_6", &CNpcScript::mafl_ray_6},
	{"mafl_ray_8", &CNpcScript::mafl_ray_8},
	{"mafl_ray_9", &CNpcScript::mafl_ray_9},
	{"mafl_ray_10", &CNpcScript::mafl_ray_10},
	{"mafl_forestmanager_0", &CNpcScript::mafl_forestmanager_0},
	{"mafl_forestmanager_1", &CNpcScript::mafl_forestmanager_1},
	{"mafl_forestmanager_2", &CNpcScript::mafl_forestmanager_2},
	{"mafl_forestmanager_3", &CNpcScript::mafl_forestmanager_3},
	{"mafl_forestmanager_4", &CNpcScript::mafl_forestmanager_4},
	{"mafl_forestmanager_5", &CNpcScript::mafl_forestmanager_5},
	{"mafl_forestmanager_6", &CNpcScript::mafl_forestmanager_6},
	{"mafl_forestmanager_8", &CNpcScript::mafl_forestmanager_8},
	{"mafl_forestmanager_9", &CNpcScript::mafl_forestmanager_9},
	{"mafl_forestmanager_10", &CNpcScript::mafl_forestmanager_10},
	{"mafl_collectormanager_0", &CNpcScript::mafl_collectormanager_0},
	{"mafl_collectormanager_1", &CNpcScript::mafl_collectormanager_1},
	{"mafl_collectormanager_2", &CNpcScript::mafl_collectormanager_2},
	{"mafl_collectormanager_3", &CNpcScript::mafl_collectormanager_3},
	{"mafl_collectormanager_4", &CNpcScript::mafl_collectormanager_4},
	{"mafl_collectormanager_5", &CNpcScript::mafl_collectormanager_5},
	{"mafl_collectormanager_6", &CNpcScript::mafl_collectormanager_6},
	{"mafl_collectormanager_8", &CNpcScript::mafl_collectormanager_8},
	{"mafl_collectormanager_9", &CNpcScript::mafl_collectormanager_9},
	{"mafl_collectormanager_10", &CNpcScript::mafl_collectormanager_10},
	{"masa_karin_event_0", &CNpcScript::masa_karin_event_0},
	{"masa_karin_event_1", &CNpcScript::masa_karin_event_1},
	{"masa_karin_event_2", &CNpcScript::masa_karin_event_2},
	{"masa_karin_event_3", &CNpcScript::masa_karin_event_3},
	{"masa_karin_event_4", &CNpcScript::masa_karin_event_4},
	{"masa_karin_event_5", &CNpcScript::masa_karin_event_5},
	{"masa_karin_event_6", &CNpcScript::masa_karin_event_6},
	{"masa_karin_event_8", &CNpcScript::masa_karin_event_8},
	{"masa_karin_event_9", &CNpcScript::masa_karin_event_9},
	{"masa_karin_event_10", &CNpcScript::masa_karin_event_10},
	{"mafl_priest_0", &CNpcScript::mafl_priest_0},
	{"mafl_priest_1", &CNpcScript::mafl_priest_1},
	{"mafl_priest_2", &CNpcScript::mafl_priest_2},
	{"mafl_priest_3", &CNpcScript::mafl_priest_3},
	{"mafl_priest_4", &CNpcScript::mafl_priest_4},
	{"mafl_priest_5", &CNpcScript::mafl_priest_5},
	{"mafl_priest_6", &CNpcScript::mafl_priest_6},
	{"mafl_priest_8", &CNpcScript::mafl_priest_8},
	{"mafl_priest_9", &CNpcScript::mafl_priest_9},
	{"mafl_priest_10", &CNpcScript::mafl_priest_10},
	{"mafl_prepaidcard_0", &CNpcScript::mafl_prepaidcard_0},
	{"mafl_prepaidcard_1", &CNpcScript::mafl_prepaidcard_1},
	{"mafl_prepaidcard_2", &CNpcScript::mafl_prepaidcard_2},
	{"mafl_prepaidcard_3", &CNpcScript::mafl_prepaidcard_3},
	{"mafl_prepaidcard_4", &CNpcScript::mafl_prepaidcard_4},
	{"mafl_prepaidcard_5", &CNpcScript::mafl_prepaidcard_5},
	{"mafl_prepaidcard_6", &CNpcScript::mafl_prepaidcard_6},
	{"mafl_prepaidcard_8", &CNpcScript::mafl_prepaidcard_8},
	{"mafl_prepaidcard_9", &CNpcScript::mafl_prepaidcard_9},
	{"mafl_prepaidcard_10", &CNpcScript::mafl_prepaidcard_10},
	{"mafl_july03_0", &CNpcScript::mafl_july03_0},
	{"mafl_july03_1", &CNpcScript::mafl_july03_1},
	{"mafl_july03_2", &CNpcScript::mafl_july03_2},
	{"mafl_july03_3", &CNpcScript::mafl_july03_3},
	{"mafl_july03_4", &CNpcScript::mafl_july03_4},
	{"mafl_july03_5", &CNpcScript::mafl_july03_5},
	{"mafl_july03_6", &CNpcScript::mafl_july03_6},
	{"mafl_july03_8", &CNpcScript::mafl_july03_8},
	{"mafl_july03_9", &CNpcScript::mafl_july03_9},
	{"mafl_july03_10", &CNpcScript::mafl_july03_10},
	{"mafl_july04_0", &CNpcScript::mafl_july04_0},
	{"mafl_july04_1", &CNpcScript::mafl_july04_1},
	{"mafl_july04_2", &CNpcScript::mafl_july04_2},
	{"mafl_july04_3", &CNpcScript::mafl_july04_3},
	{"mafl_july04_4", &CNpcScript::mafl_july04_4},
	{"mafl_july04_5", &CNpcScript::mafl_july04_5},
	{"mafl_july04_6", &CNpcScript::mafl_july04_6},
	{"mafl_july04_8", &CNpcScript::mafl_july04_8},
	{"mafl_july04_9", &CNpcScript::mafl_july04_9},
	{"mafl_july04_10", &CNpcScript::mafl_july04_10},
	{"mafl_secretroom_east_0", &CNpcScript::mafl_secretroom_east_0},
	{"mafl_secretroom_east_1", &CNpcScript::mafl_secretroom_east_1},
	{"mafl_secretroom_east_2", &CNpcScript::mafl_secretroom_east_2},
	{"mafl_secretroom_east_3", &CNpcScript::mafl_secretroom_east_3},
	{"mafl_secretroom_east_4", &CNpcScript::mafl_secretroom_east_4},
	{"mafl_secretroom_east_5", &CNpcScript::mafl_secretroom_east_5},
	{"mafl_secretroom_east_6", &CNpcScript::mafl_secretroom_east_6},
	{"mafl_secretroom_east_8", &CNpcScript::mafl_secretroom_east_8},
	{"mafl_secretroom_east_9", &CNpcScript::mafl_secretroom_east_9},
	{"mafl_secretroom_east_10", &CNpcScript::mafl_secretroom_east_10},
	{"mafl_secretroom_west_0", &CNpcScript::mafl_secretroom_west_0},
	{"mafl_secretroom_west_1", &CNpcScript::mafl_secretroom_west_1},
	{"mafl_secretroom_west_2", &CNpcScript::mafl_secretroom_west_2},
	{"mafl_secretroom_west_3", &CNpcScript::mafl_secretroom_west_3},
	{"mafl_secretroom_west_4", &CNpcScript::mafl_secretroom_west_4},
	{"mafl_secretroom_west_5", &CNpcScript::mafl_secretroom_west_5},
	{"mafl_secretroom_west_6", &CNpcScript::mafl_secretroom_west_6},
	{"mafl_secretroom_west_8", &CNpcScript::mafl_secretroom_west_8},
	{"mafl_secretroom_west_9", &CNpcScript::mafl_secretroom_west_9},
	{"mafl_secretroom_west_10", &CNpcScript::mafl_secretroom_west_10},
	{"mafl_cardmaster_0", &CNpcScript::mafl_cardmaster_0},
	{"mafl_cardmaster_1", &CNpcScript::mafl_cardmaster_1},
	{"mafl_cardmaster_2", &CNpcScript::mafl_cardmaster_2},
	{"mafl_cardmaster_3", &CNpcScript::mafl_cardmaster_3},
	{"mafl_cardmaster_4", &CNpcScript::mafl_cardmaster_4},
	{"mafl_cardmaster_5", &CNpcScript::mafl_cardmaster_5},
	{"mafl_cardmaster_6", &CNpcScript::mafl_cardmaster_6},
	{"mafl_cardmaster_8", &CNpcScript::mafl_cardmaster_8},
	{"mafl_cardmaster_9", &CNpcScript::mafl_cardmaster_9},
	{"mafl_cardmaster_10", &CNpcScript::mafl_cardmaster_10},
	{"mafl_peach_0", &CNpcScript::mafl_peach_0},
	{"mafl_peach_1", &CNpcScript::mafl_peach_1},
	{"mafl_peach_2", &CNpcScript::mafl_peach_2},
	{"mafl_peach_3", &CNpcScript::mafl_peach_3},
	{"mafl_peach_4", &CNpcScript::mafl_peach_4},
	{"mafl_peach_5", &CNpcScript::mafl_peach_5},
	{"mafl_peach_6", &CNpcScript::mafl_peach_6},
	{"mafl_peach_8", &CNpcScript::mafl_peach_8},
	{"mafl_peach_9", &CNpcScript::mafl_peach_9},
	{"mafl_peach_10", &CNpcScript::mafl_peach_10},
	{"mafl_miniwheel_0", &CNpcScript::mafl_miniwheel_0},
	{"mafl_miniwheel_1", &CNpcScript::mafl_miniwheel_1},
	{"mafl_miniwheel_2", &CNpcScript::mafl_miniwheel_2},
	{"mafl_miniwheel_3", &CNpcScript::mafl_miniwheel_3},
	{"mafl_miniwheel_4", &CNpcScript::mafl_miniwheel_4},
	{"mafl_miniwheel_5", &CNpcScript::mafl_miniwheel_5},
	{"mafl_miniwheel_6", &CNpcScript::mafl_miniwheel_6},
	{"mafl_miniwheel_8", &CNpcScript::mafl_miniwheel_8},
	{"mafl_miniwheel_9", &CNpcScript::mafl_miniwheel_9},
	{"mafl_miniwheel_10", &CNpcScript::mafl_miniwheel_10},
	{"mafl_eventball_0", &CNpcScript::mafl_eventball_0},
	{"mafl_eventball_1", &CNpcScript::mafl_eventball_1},
	{"mafl_eventball_2", &CNpcScript::mafl_eventball_2},
	{"mafl_eventball_3", &CNpcScript::mafl_eventball_3},
	{"mafl_eventball_4", &CNpcScript::mafl_eventball_4},
	{"mafl_eventball_5", &CNpcScript::mafl_eventball_5},
	{"mafl_eventball_6", &CNpcScript::mafl_eventball_6},
	{"mafl_eventball_8", &CNpcScript::mafl_eventball_8},
	{"mafl_eventball_9", &CNpcScript::mafl_eventball_9},
	{"mafl_eventball_10", &CNpcScript::mafl_eventball_10},
	{"mafl_redrobegirl_0", &CNpcScript::mafl_redrobegirl_0},
	{"mafl_redrobegirl_1", &CNpcScript::mafl_redrobegirl_1},
	{"mafl_redrobegirl_2", &CNpcScript::mafl_redrobegirl_2},
	{"mafl_redrobegirl_3", &CNpcScript::mafl_redrobegirl_3},
	{"mafl_redrobegirl_4", &CNpcScript::mafl_redrobegirl_4},
	{"mafl_redrobegirl_5", &CNpcScript::mafl_redrobegirl_5},
	{"mafl_redrobegirl_6", &CNpcScript::mafl_redrobegirl_6},
	{"mafl_redrobegirl_8", &CNpcScript::mafl_redrobegirl_8},
	{"mafl_redrobegirl_9", &CNpcScript::mafl_redrobegirl_9},
	{"mafl_redrobegirl_10", &CNpcScript::mafl_redrobegirl_10},
	{"mafl_charlie_0", &CNpcScript::mafl_charlie_0},
	{"mafl_charlie_1", &CNpcScript::mafl_charlie_1},
	{"mafl_charlie_2", &CNpcScript::mafl_charlie_2},
	{"mafl_charlie_3", &CNpcScript::mafl_charlie_3},
	{"mafl_charlie_4", &CNpcScript::mafl_charlie_4},
	{"mafl_charlie_5", &CNpcScript::mafl_charlie_5},
	{"mafl_charlie_6", &CNpcScript::mafl_charlie_6},
	{"mafl_charlie_8", &CNpcScript::mafl_charlie_8},
	{"mafl_charlie_9", &CNpcScript::mafl_charlie_9},
	{"mafl_charlie_10", &CNpcScript::mafl_charlie_10},
	{"mafl_snowman01_0", &CNpcScript::mafl_snowman01_0},
	{"mafl_snowman01_1", &CNpcScript::mafl_snowman01_1},
	{"mafl_snowman01_2", &CNpcScript::mafl_snowman01_2},
	{"mafl_snowman01_3", &CNpcScript::mafl_snowman01_3},
	{"mafl_snowman01_4", &CNpcScript::mafl_snowman01_4},
	{"mafl_snowman01_5", &CNpcScript::mafl_snowman01_5},
	{"mafl_snowman01_6", &CNpcScript::mafl_snowman01_6},
	{"mafl_snowman01_8", &CNpcScript::mafl_snowman01_8},
	{"mafl_snowman01_9", &CNpcScript::mafl_snowman01_9},
	{"mafl_snowman01_10", &CNpcScript::mafl_snowman01_10},
	{"mafl_snowman02_0", &CNpcScript::mafl_snowman02_0},
	{"mafl_snowman02_1", &CNpcScript::mafl_snowman02_1},
	{"mafl_snowman02_2", &CNpcScript::mafl_snowman02_2},
	{"mafl_snowman02_3", &CNpcScript::mafl_snowman02_3},
	{"mafl_snowman02_4", &CNpcScript::mafl_snowman02_4},
	{"mafl_snowman02_5", &CNpcScript::mafl_snowman02_5},
	{"mafl_snowman02_6", &CNpcScript::mafl_snowman02_6},
	{"mafl_snowman02_8", &CNpcScript::mafl_snowman02_8},
	{"mafl_snowman02_9", &CNpcScript::mafl_snowman02_9},
	{"mafl_snowman02_10", &CNpcScript::mafl_snowman02_10},
	{"mafl_grahoo_0", &CNpcScript::mafl_grahoo_0},
	{"mafl_grahoo_1", &CNpcScript::mafl_grahoo_1},
	{"mafl_grahoo_2", &CNpcScript::mafl_grahoo_2},
	{"mafl_grahoo_3", &CNpcScript::mafl_grahoo_3},
	{"mafl_grahoo_4", &CNpcScript::mafl_grahoo_4},
	{"mafl_grahoo_5", &CNpcScript::mafl_grahoo_5},
	{"mafl_grahoo_6", &CNpcScript::mafl_grahoo_6},
	{"mafl_grahoo_8", &CNpcScript::mafl_grahoo_8},
	{"mafl_grahoo_9", &CNpcScript::mafl_grahoo_9},
	{"mafl_grahoo_10", &CNpcScript::mafl_grahoo_10},
	{"mafl_aberoo_0", &CNpcScript::mafl_aberoo_0},
	{"mafl_aberoo_1", &CNpcScript::mafl_aberoo_1},
	{"mafl_aberoo_2", &CNpcScript::mafl_aberoo_2},
	{"mafl_aberoo_3", &CNpcScript::mafl_aberoo_3},
	{"mafl_aberoo_4", &CNpcScript::mafl_aberoo_4},
	{"mafl_aberoo_5", &CNpcScript::mafl_aberoo_5},
	{"mafl_aberoo_6", &CNpcScript::mafl_aberoo_6},
	{"mafl_aberoo_8", &CNpcScript::mafl_aberoo_8},
	{"mafl_aberoo_9", &CNpcScript::mafl_aberoo_9},
	{"mafl_aberoo_10", &CNpcScript::mafl_aberoo_10},
	{"mafl_cook01_0", &CNpcScript::mafl_cook01_0},
	{"mafl_cook01_1", &CNpcScript::mafl_cook01_1},
	{"mafl_cook01_2", &CNpcScript::mafl_cook01_2},
	{"mafl_cook01_3", &CNpcScript::mafl_cook01_3},
	{"mafl_cook01_4", &CNpcScript::mafl_cook01_4},
	{"mafl_cook01_5", &CNpcScript::mafl_cook01_5},
	{"mafl_cook01_6", &CNpcScript::mafl_cook01_6},
	{"mafl_cook01_8", &CNpcScript::mafl_cook01_8},
	{"mafl_cook01_9", &CNpcScript::mafl_cook01_9},
	{"mafl_cook01_10", &CNpcScript::mafl_cook01_10},
	{"mafl_alice_0", &CNpcScript::mafl_alice_0},
	{"mafl_alice_1", &CNpcScript::mafl_alice_1},
	{"mafl_alice_2", &CNpcScript::mafl_alice_2},
	{"mafl_alice_3", &CNpcScript::mafl_alice_3},
	{"mafl_alice_4", &CNpcScript::mafl_alice_4},
	{"mafl_alice_5", &CNpcScript::mafl_alice_5},
	{"mafl_alice_6", &CNpcScript::mafl_alice_6},
	{"mafl_alice_8", &CNpcScript::mafl_alice_8},
	{"mafl_alice_9", &CNpcScript::mafl_alice_9},
	{"mafl_alice_10", &CNpcScript::mafl_alice_10},
	{"mafl_oasis_0", &CNpcScript::mafl_oasis_0},
	{"mafl_oasis_1", &CNpcScript::mafl_oasis_1},
	{"mafl_oasis_2", &CNpcScript::mafl_oasis_2},
	{"mafl_oasis_3", &CNpcScript::mafl_oasis_3},
	{"mafl_oasis_4", &CNpcScript::mafl_oasis_4},
	{"mafl_oasis_5", &CNpcScript::mafl_oasis_5},
	{"mafl_oasis_6", &CNpcScript::mafl_oasis_6},
	{"mafl_oasis_8", &CNpcScript::mafl_oasis_8},
	{"mafl_oasis_9", &CNpcScript::mafl_oasis_9},
	{"mafl_oasis_10", &CNpcScript::mafl_oasis_10},
	{"maha_jano_0", &CNpcScript::maha_jano_0},
	{"maha_jano_1", &CNpcScript::maha_jano_1},
	{"maha_jano_2", &CNpcScript::maha_jano_2},
	{"maha_jano_3", &CNpcScript::maha_jano_3},
	{"maha_jano_4", &CNpcScript::maha_jano_4},
	{"maha_jano_5", &CNpcScript::maha_jano_5},
	{"maha_jano_6", &CNpcScript::maha_jano_6},
	{"maha_jano_8", &CNpcScript::maha_jano_8},
	{"maha_jano_9", &CNpcScript::maha_jano_9},
	{"maha_jano_10", &CNpcScript::maha_jano_10},
	{"maha_luca_0", &CNpcScript::maha_luca_0},
	{"maha_luca_1", &CNpcScript::maha_luca_1},
	{"maha_luca_2", &CNpcScript::maha_luca_2},
	{"maha_luca_3", &CNpcScript::maha_luca_3},
	{"maha_luca_4", &CNpcScript::maha_luca_4},
	{"maha_luca_5", &CNpcScript::maha_luca_5},
	{"maha_luca_6", &CNpcScript::maha_luca_6},
	{"maha_luca_8", &CNpcScript::maha_luca_8},
	{"maha_luca_9", &CNpcScript::maha_luca_9},
	{"maha_luca_10", &CNpcScript::maha_luca_10},
	{"maha_vespu_0", &CNpcScript::maha_vespu_0},
	{"maha_vespu_1", &CNpcScript::maha_vespu_1},
	{"maha_vespu_2", &CNpcScript::maha_vespu_2},
	{"maha_vespu_3", &CNpcScript::maha_vespu_3},
	{"maha_vespu_4", &CNpcScript::maha_vespu_4},
	{"maha_vespu_5", &CNpcScript::maha_vespu_5},
	{"maha_vespu_6", &CNpcScript::maha_vespu_6},
	{"maha_vespu_8", &CNpcScript::maha_vespu_8},
	{"maha_vespu_9", &CNpcScript::maha_vespu_9},
	{"maha_vespu_10", &CNpcScript::maha_vespu_10},
	{"mada_esof_0", &CNpcScript::mada_esof_0},
	{"mada_esof_1", &CNpcScript::mada_esof_1},
	{"mada_esof_2", &CNpcScript::mada_esof_2},
	{"mada_esof_3", &CNpcScript::mada_esof_3},
	{"mada_esof_4", &CNpcScript::mada_esof_4},
	{"mada_esof_5", &CNpcScript::mada_esof_5},
	{"mada_esof_6", &CNpcScript::mada_esof_6},
	{"mada_esof_8", &CNpcScript::mada_esof_8},
	{"mada_esof_9", &CNpcScript::mada_esof_9},
	{"mada_esof_10", &CNpcScript::mada_esof_10},
	{"maha_ryan_0", &CNpcScript::maha_ryan_0},
	{"maha_ryan_1", &CNpcScript::maha_ryan_1},
	{"maha_ryan_2", &CNpcScript::maha_ryan_2},
	{"maha_ryan_3", &CNpcScript::maha_ryan_3},
	{"maha_ryan_4", &CNpcScript::maha_ryan_4},
	{"maha_ryan_5", &CNpcScript::maha_ryan_5},
	{"maha_ryan_6", &CNpcScript::maha_ryan_6},
	{"maha_ryan_8", &CNpcScript::maha_ryan_8},
	{"maha_ryan_9", &CNpcScript::maha_ryan_9},
	{"maha_ryan_10", &CNpcScript::maha_ryan_10},
	{"maha_lasa_0", &CNpcScript::maha_lasa_0},
	{"maha_lasa_1", &CNpcScript::maha_lasa_1},
	{"maha_lasa_2", &CNpcScript::maha_lasa_2},
	{"maha_lasa_3", &CNpcScript::maha_lasa_3},
	{"maha_lasa_4", &CNpcScript::maha_lasa_4},
	{"maha_lasa_5", &CNpcScript::maha_lasa_5},
	{"maha_lasa_6", &CNpcScript::maha_lasa_6},
	{"maha_lasa_8", &CNpcScript::maha_lasa_8},
	{"maha_lasa_9", &CNpcScript::maha_lasa_9},
	{"maha_lasa_10", &CNpcScript::maha_lasa_10},
	{"mafl_meron_0", &CNpcScript::mafl_meron_0},
	{"mafl_meron_1", &CNpcScript::mafl_meron_1},
	{"mafl_meron_2", &CNpcScript::mafl_meron_2},
	{"mafl_meron_3", &CNpcScript::mafl_meron_3},
	{"mafl_meron_4", &CNpcScript::mafl_meron_4},
	{"mafl_meron_5", &CNpcScript::mafl_meron_5},
	{"mafl_meron_6", &CNpcScript::mafl_meron_6},
	{"mafl_meron_8", &CNpcScript::mafl_meron_8},
	{"mafl_meron_9", &CNpcScript::mafl_meron_9},
	{"mafl_meron_10", &CNpcScript::mafl_meron_10},	
	{"mafl_august_0", &CNpcScript::mafl_august_0},
	{"mafl_august_1", &CNpcScript::mafl_august_1},
	{"mafl_august_2", &CNpcScript::mafl_august_2},
	{"mafl_august_3", &CNpcScript::mafl_august_3},
	{"mafl_august_4", &CNpcScript::mafl_august_4},
	{"mafl_august_5", &CNpcScript::mafl_august_5},
	{"mafl_august_6", &CNpcScript::mafl_august_6},
	{"mafl_august_8", &CNpcScript::mafl_august_8},
	{"mafl_august_9", &CNpcScript::mafl_august_9},
	{"mafl_august_10", &CNpcScript::mafl_august_10},	
	{"mafl_picksirose_0", &CNpcScript::mafl_picksirose_0},
	{"mafl_picksirose_1", &CNpcScript::mafl_picksirose_1},
	{"mafl_picksirose_2", &CNpcScript::mafl_picksirose_2},
	{"mafl_picksirose_3", &CNpcScript::mafl_picksirose_3},
	{"mafl_picksirose_4", &CNpcScript::mafl_picksirose_4},
	{"mafl_picksirose_5", &CNpcScript::mafl_picksirose_5},
	{"mafl_picksirose_6", &CNpcScript::mafl_picksirose_6},
	{"mafl_picksirose_8", &CNpcScript::mafl_picksirose_8},
	{"mafl_picksirose_9", &CNpcScript::mafl_picksirose_9},
	{"mafl_picksirose_10", &CNpcScript::mafl_picksirose_10},
	{"mafl_heroevent01_0", &CNpcScript::mafl_heroevent01_0},
	{"mafl_heroevent01_1", &CNpcScript::mafl_heroevent01_1},
	{"mafl_heroevent01_2", &CNpcScript::mafl_heroevent01_2},
	{"mafl_heroevent01_3", &CNpcScript::mafl_heroevent01_3},
	{"mafl_heroevent01_4", &CNpcScript::mafl_heroevent01_4},
	{"mafl_heroevent01_5", &CNpcScript::mafl_heroevent01_5},
	{"mafl_heroevent01_6", &CNpcScript::mafl_heroevent01_6},
	{"mafl_heroevent01_8", &CNpcScript::mafl_heroevent01_8},
	{"mafl_heroevent01_9", &CNpcScript::mafl_heroevent01_9},
	{"mafl_heroevent01_10", &CNpcScript::mafl_heroevent01_10},
	{"mafl_vanharlen_0", &CNpcScript::mafl_vanharlen_0},
	{"mafl_vanharlen_1", &CNpcScript::mafl_vanharlen_1},
	{"mafl_vanharlen_2", &CNpcScript::mafl_vanharlen_2},
	{"mafl_vanharlen_3", &CNpcScript::mafl_vanharlen_3},
	{"mafl_vanharlen_4", &CNpcScript::mafl_vanharlen_4},
	{"mafl_vanharlen_5", &CNpcScript::mafl_vanharlen_5},
	{"mafl_vanharlen_6", &CNpcScript::mafl_vanharlen_6},
	{"mafl_vanharlen_8", &CNpcScript::mafl_vanharlen_8},
	{"mafl_vanharlen_9", &CNpcScript::mafl_vanharlen_9},
	{"mafl_vanharlen_10", &CNpcScript::mafl_vanharlen_10},
	{"mafl_ete_0", &CNpcScript::mafl_ete_0},
	{"mafl_ete_1", &CNpcScript::mafl_ete_1},
	{"mafl_ete_2", &CNpcScript::mafl_ete_2},
	{"mafl_ete_3", &CNpcScript::mafl_ete_3},
	{"mafl_ete_4", &CNpcScript::mafl_ete_4},
	{"mafl_ete_5", &CNpcScript::mafl_ete_5},
	{"mafl_ete_6", &CNpcScript::mafl_ete_6},
	{"mafl_ete_8", &CNpcScript::mafl_ete_8},
	{"mafl_ete_9", &CNpcScript::mafl_ete_9},
	{"mafl_ete_10", &CNpcScript::mafl_ete_10},
	{"mafl_tonge_0", &CNpcScript::mafl_tonge_0},
	{"mafl_tonge_1", &CNpcScript::mafl_tonge_1},
	{"mafl_tonge_2", &CNpcScript::mafl_tonge_2},
	{"mafl_tonge_3", &CNpcScript::mafl_tonge_3},
	{"mafl_tonge_4", &CNpcScript::mafl_tonge_4},
	{"mafl_tonge_5", &CNpcScript::mafl_tonge_5},
	{"mafl_tonge_6", &CNpcScript::mafl_tonge_6},
	{"mafl_tonge_8", &CNpcScript::mafl_tonge_8},
	{"mafl_tonge_9", &CNpcScript::mafl_tonge_9},
	{"mafl_tonge_10", &CNpcScript::mafl_tonge_10},
	{"mafl_missvtn_0", &CNpcScript::mafl_missvtn_0},
	{"mafl_missvtn_1", &CNpcScript::mafl_missvtn_1},
	{"mafl_missvtn_2", &CNpcScript::mafl_missvtn_2},
	{"mafl_missvtn_3", &CNpcScript::mafl_missvtn_3},
	{"mafl_missvtn_4", &CNpcScript::mafl_missvtn_4},
	{"mafl_missvtn_5", &CNpcScript::mafl_missvtn_5},
	{"mafl_missvtn_6", &CNpcScript::mafl_missvtn_6},
	{"mafl_missvtn_8", &CNpcScript::mafl_missvtn_8},
	{"mafl_missvtn_9", &CNpcScript::mafl_missvtn_9},
	{"mafl_missvtn_10", &CNpcScript::mafl_missvtn_10},
	{"mafl_teleporter_03nomal_0", &CNpcScript::mafl_teleporter_03nomal_0},
	{"mafl_teleporter_03nomal_1", &CNpcScript::mafl_teleporter_03nomal_1},
	{"mafl_teleporter_03nomal_2", &CNpcScript::mafl_teleporter_03nomal_2},
	{"mafl_teleporter_03nomal_3", &CNpcScript::mafl_teleporter_03nomal_3},
	{"mafl_teleporter_03nomal_4", &CNpcScript::mafl_teleporter_03nomal_4},
	{"mafl_teleporter_03nomal_5", &CNpcScript::mafl_teleporter_03nomal_5},
	{"mafl_teleporter_03nomal_6", &CNpcScript::mafl_teleporter_03nomal_6},
	{"mafl_teleporter_03nomal_8", &CNpcScript::mafl_teleporter_03nomal_8},
	{"mafl_teleporter_03nomal_9", &CNpcScript::mafl_teleporter_03nomal_9},
	{"mafl_teleporter_03nomal_10", &CNpcScript::mafl_teleporter_03nomal_10},
	{"mafl_guildhousesale_0", &CNpcScript::mafl_guildhousesale_0},
	{"mafl_guildhousesale_1", &CNpcScript::mafl_guildhousesale_1},
	{"mafl_guildhousesale_2", &CNpcScript::mafl_guildhousesale_2},
	{"mafl_guildhousesale_3", &CNpcScript::mafl_guildhousesale_3},
	{"mafl_guildhousesale_4", &CNpcScript::mafl_guildhousesale_4},
	{"mafl_guildhousesale_5", &CNpcScript::mafl_guildhousesale_5},
	{"mafl_guildhousesale_6", &CNpcScript::mafl_guildhousesale_6},
	{"mafl_guildhousesale_8", &CNpcScript::mafl_guildhousesale_8},
	{"mafl_guildhousesale_9", &CNpcScript::mafl_guildhousesale_9},
	{"mafl_guildhousesale_10", &CNpcScript::mafl_guildhousesale_10},
	{"mafl_christmas_romeo_0", &CNpcScript::mafl_christmas_romeo_0},
	{"mafl_christmas_romeo_1", &CNpcScript::mafl_christmas_romeo_1},
	{"mafl_christmas_romeo_2", &CNpcScript::mafl_christmas_romeo_2},
	{"mafl_christmas_romeo_3", &CNpcScript::mafl_christmas_romeo_3},
	{"mafl_christmas_romeo_4", &CNpcScript::mafl_christmas_romeo_4},
	{"mafl_christmas_romeo_5", &CNpcScript::mafl_christmas_romeo_5},
	{"mafl_christmas_romeo_6", &CNpcScript::mafl_christmas_romeo_6},
	{"mafl_christmas_romeo_8", &CNpcScript::mafl_christmas_romeo_8},
	{"mafl_christmas_romeo_9", &CNpcScript::mafl_christmas_romeo_9},
	{"mafl_Christmas_Romeo_10", &CNpcScript::mafl_christmas_romeo_10},
	{"mafl_christmas_juliet_0", &CNpcScript::mafl_christmas_juliet_0},
	{"mafl_christmas_juliet_1", &CNpcScript::mafl_christmas_juliet_1},
	{"mafl_christmas_juliet_2", &CNpcScript::mafl_christmas_juliet_2},
	{"mafl_christmas_juliet_3", &CNpcScript::mafl_christmas_juliet_3},
	{"mafl_christmas_juliet_4", &CNpcScript::mafl_christmas_juliet_4},
	{"mafl_christmas_juliet_5", &CNpcScript::mafl_christmas_juliet_5},
	{"mafl_christmas_juliet_6", &CNpcScript::mafl_christmas_juliet_6},
	{"mafl_christmas_juliet_8", &CNpcScript::mafl_christmas_juliet_8},
	{"mafl_christmas_juliet_9", &CNpcScript::mafl_christmas_juliet_9},
	{"mafl_christmas_juliet_10", &CNpcScript::mafl_christmas_juliet_10},
	{"mada_icinis_0", &CNpcScript::mada_icinis_0},
	{"mada_icinis_1", &CNpcScript::mada_icinis_1},
	{"mada_icinis_2", &CNpcScript::mada_icinis_2},
	{"mada_icinis_3", &CNpcScript::mada_icinis_3},
	{"mada_icinis_4", &CNpcScript::mada_icinis_4},
	{"mada_icinis_5", &CNpcScript::mada_icinis_5},
	{"mada_icinis_6", &CNpcScript::mada_icinis_6},
	{"mada_icinis_8", &CNpcScript::mada_icinis_8},
	{"mada_icinis_9", &CNpcScript::mada_icinis_9},
	{"mada_icinis_10", &CNpcScript::mada_icinis_10},
	{"mafl_commoner03_0", &CNpcScript::mafl_commoner03_0},
	{"mafl_commoner03_1", &CNpcScript::mafl_commoner03_1},
	{"mafl_commoner03_2", &CNpcScript::mafl_commoner03_2},
	{"mafl_commoner03_3", &CNpcScript::mafl_commoner03_3},
	{"mafl_commoner03_4", &CNpcScript::mafl_commoner03_4},
	{"mafl_commoner03_5", &CNpcScript::mafl_commoner03_5},
	{"mafl_commoner03_6", &CNpcScript::mafl_commoner03_6},
	{"mafl_commoner03_8", &CNpcScript::mafl_commoner03_8},
	{"mafl_commoner03_9", &CNpcScript::mafl_commoner03_9},
	{"mafl_commoner03_10", &CNpcScript::mafl_commoner03_10},	
	{"mafl_teleporter_03veteran_0", &CNpcScript::mafl_teleporter_03veteran_0},
	{"mafl_teleporter_03veteran_1", &CNpcScript::mafl_teleporter_03veteran_1},
	{"mafl_teleporter_03veteran_2", &CNpcScript::mafl_teleporter_03veteran_2},
	{"mafl_teleporter_03veteran_3", &CNpcScript::mafl_teleporter_03veteran_3},
	{"mafl_teleporter_03veteran_4", &CNpcScript::mafl_teleporter_03veteran_4},
	{"mafl_teleporter_03veteran_5", &CNpcScript::mafl_teleporter_03veteran_5},
	{"mafl_teleporter_03veteran_6", &CNpcScript::mafl_teleporter_03veteran_6},
	{"mafl_teleporter_03veteran_8", &CNpcScript::mafl_teleporter_03veteran_8},
	{"mafl_teleporter_03veteran_9", &CNpcScript::mafl_teleporter_03veteran_9},
	{"mafl_teleporter_03veteran_10", &CNpcScript::mafl_teleporter_03veteran_10},
	{"mafl_teleporter_2_0", &CNpcScript::mafl_teleporter_2_0},
	{"mafl_teleporter_2_1", &CNpcScript::mafl_teleporter_2_1},
	{"mafl_teleporter_2_2", &CNpcScript::mafl_teleporter_2_2},
	{"mafl_teleporter_2_3", &CNpcScript::mafl_teleporter_2_3},
	{"mafl_teleporter_2_4", &CNpcScript::mafl_teleporter_2_4},
	{"mafl_teleporter_2_5", &CNpcScript::mafl_teleporter_2_5},
	{"mafl_teleporter_2_6", &CNpcScript::mafl_teleporter_2_6},
	{"mafl_teleporter_2_8", &CNpcScript::mafl_teleporter_2_8},
	{"mafl_teleporter_2_9", &CNpcScript::mafl_teleporter_2_9},
	{"mafl_teleporter_2_10", &CNpcScript::mafl_teleporter_2_10},
	{"mada_laydian_0", &CNpcScript::mada_laydian_0},
	{"mada_laydian_1", &CNpcScript::mada_laydian_1},
	{"mada_laydian_2", &CNpcScript::mada_laydian_2},
	{"mada_laydian_3", &CNpcScript::mada_laydian_3},
	{"mada_laydian_4", &CNpcScript::mada_laydian_4},
	{"mada_laydian_5", &CNpcScript::mada_laydian_5},
	{"mada_laydian_6", &CNpcScript::mada_laydian_6},
	{"mada_laydian_8", &CNpcScript::mada_laydian_8},
	{"mada_laydian_9", &CNpcScript::mada_laydian_9},
	{"mada_laydian_10", &CNpcScript::mada_laydian_10},
	{"mafl_cupid_0", &CNpcScript::mafl_cupid_0},
	{"mafl_cupid_1", &CNpcScript::mafl_cupid_1},
	{"mafl_cupid_2", &CNpcScript::mafl_cupid_2},
	{"mafl_cupid_3", &CNpcScript::mafl_cupid_3},
	{"mafl_cupid_4", &CNpcScript::mafl_cupid_4},
	{"mafl_cupid_5", &CNpcScript::mafl_cupid_5},
	{"mafl_cupid_6", &CNpcScript::mafl_cupid_6},
	{"mafl_cupid_8", &CNpcScript::mafl_cupid_8},
	{"mafl_cupid_9", &CNpcScript::mafl_cupid_9},
	{"mafl_cupid_10", &CNpcScript::mafl_cupid_10},
	{"mafl_correspondent_0", &CNpcScript::mafl_correspondent_0},
	{"mafl_correspondent_1", &CNpcScript::mafl_correspondent_1},
	{"mafl_correspondent_2", &CNpcScript::mafl_correspondent_2},
	{"mafl_correspondent_3", &CNpcScript::mafl_correspondent_3},
	{"mafl_correspondent_4", &CNpcScript::mafl_correspondent_4},
	{"mafl_correspondent_5", &CNpcScript::mafl_correspondent_5},
	{"mafl_correspondent_6", &CNpcScript::mafl_correspondent_6},
	{"mafl_correspondent_8", &CNpcScript::mafl_correspondent_8},
	{"mafl_correspondent_9", &CNpcScript::mafl_correspondent_9},
	{"mafl_correspondent_10", &CNpcScript::mafl_correspondent_10},
	{"mafl_newbie_0", &CNpcScript::mafl_newbie_0},
	{"mafl_newbie_1", &CNpcScript::mafl_newbie_1},
	{"mafl_newbie_2", &CNpcScript::mafl_newbie_2},
	{"mafl_newbie_3", &CNpcScript::mafl_newbie_3},
	{"mafl_newbie_4", &CNpcScript::mafl_newbie_4},
	{"mafl_newbie_5", &CNpcScript::mafl_newbie_5},
	{"mafl_newbie_6", &CNpcScript::mafl_newbie_6},
	{"mafl_newbie_8", &CNpcScript::mafl_newbie_8},
	{"mafl_newbie_9", &CNpcScript::mafl_newbie_9},
	{"mafl_newbie_10", &CNpcScript::mafl_newbie_10},
	{"mafl_bubble_0", &CNpcScript::mafl_bubble_0},
	{"mafl_bubble_1", &CNpcScript::mafl_bubble_1},
	{"mafl_bubble_2", &CNpcScript::mafl_bubble_2},
	{"mafl_bubble_3", &CNpcScript::mafl_bubble_3},
	{"mafl_bubble_4", &CNpcScript::mafl_bubble_4},
	{"mafl_bubble_5", &CNpcScript::mafl_bubble_5},
	{"mafl_bubble_6", &CNpcScript::mafl_bubble_6},
	{"mafl_bubble_8", &CNpcScript::mafl_bubble_8},
	{"mafl_bubble_9", &CNpcScript::mafl_bubble_9},
	{"mafl_bubble_10", &CNpcScript::mafl_bubble_10},
	{"mafl_leprechaun_0", &CNpcScript::mafl_leprechaun_0},
	{"mafl_leprechaun_1", &CNpcScript::mafl_leprechaun_1},
	{"mafl_leprechaun_2", &CNpcScript::mafl_leprechaun_2},
	{"mafl_leprechaun_3", &CNpcScript::mafl_leprechaun_3},
	{"mafl_leprechaun_4", &CNpcScript::mafl_leprechaun_4},
	{"mafl_leprechaun_5", &CNpcScript::mafl_leprechaun_5},
	{"mafl_leprechaun_6", &CNpcScript::mafl_leprechaun_6},
	{"mafl_leprechaun_8", &CNpcScript::mafl_leprechaun_8},
	{"mafl_leprechaun_9", &CNpcScript::mafl_leprechaun_9},
	{"mafl_leprechaun_10", &CNpcScript::mafl_leprechaun_10},
	{"mafl_cupid02_0", &CNpcScript::mafl_cupid02_0},
	{"mafl_cupid02_1", &CNpcScript::mafl_cupid02_1},
	{"mafl_cupid02_2", &CNpcScript::mafl_cupid02_2},
	{"mafl_cupid02_3", &CNpcScript::mafl_cupid02_3},
	{"mafl_cupid02_4", &CNpcScript::mafl_cupid02_4},
	{"mafl_cupid02_5", &CNpcScript::mafl_cupid02_5},
	{"mafl_cupid02_6", &CNpcScript::mafl_cupid02_6},
	{"mafl_cupid02_8", &CNpcScript::mafl_cupid02_8},
	{"mafl_cupid02_9", &CNpcScript::mafl_cupid02_9},
	{"mafl_cupid02_10", &CNpcScript::mafl_cupid02_10},
	{"mafl_Babario_0", &CNpcScript::mafl_Babario_0},
	{"mafl_Babario_1", &CNpcScript::mafl_Babario_1},
	{"mafl_Babario_2", &CNpcScript::mafl_Babario_2},
	{"mafl_Babario_3", &CNpcScript::mafl_Babario_3},
	{"mafl_Babario_4", &CNpcScript::mafl_Babario_4},
	{"mafl_Babario_5", &CNpcScript::mafl_Babario_5},
	{"mafl_Babario_6", &CNpcScript::mafl_Babario_6},
	{"mafl_Babario_8", &CNpcScript::mafl_Babario_8},
	{"mafl_Babario_9", &CNpcScript::mafl_Babario_9},
	{"mafl_Babario_10", &CNpcScript::mafl_Babario_10},
	{"mafl_bullsfestival_0", &CNpcScript::mafl_bullsfestival_0},
	{"mafl_bullsfestival_1", &CNpcScript::mafl_bullsfestival_1},
	{"mafl_bullsfestival_2", &CNpcScript::mafl_bullsfestival_2},
	{"mafl_bullsfestival_3", &CNpcScript::mafl_bullsfestival_3},
	{"mafl_bullsfestival_4", &CNpcScript::mafl_bullsfestival_4},
	{"mafl_bullsfestival_5", &CNpcScript::mafl_bullsfestival_5},
	{"mafl_bullsfestival_6", &CNpcScript::mafl_bullsfestival_6},
	{"mafl_bullsfestival_8", &CNpcScript::mafl_bullsfestival_8},
	{"mafl_bullsfestival_9", &CNpcScript::mafl_bullsfestival_9},
	{"mafl_bullsfestival_10", &CNpcScript::mafl_bullsfestival_10},
	{"mafl_romina_0", &CNpcScript::mafl_romina_0},
	{"mafl_romina_1", &CNpcScript::mafl_romina_1},
	{"mafl_romina_2", &CNpcScript::mafl_romina_2},
	{"mafl_romina_3", &CNpcScript::mafl_romina_3},
	{"mafl_romina_4", &CNpcScript::mafl_romina_4},
	{"mafl_romina_5", &CNpcScript::mafl_romina_5},
	{"mafl_romina_6", &CNpcScript::mafl_romina_6},
	{"mafl_romina_8", &CNpcScript::mafl_romina_8},
	{"mafl_romina_9", &CNpcScript::mafl_romina_9},
	{"mafl_romina_10", &CNpcScript::mafl_romina_10},
	{"maew_cheirang_0", &CNpcScript::maew_cheirang_0},
	{"maew_cheirang_1", &CNpcScript::maew_cheirang_1},
	{"maew_cheirang_2", &CNpcScript::maew_cheirang_2},
	{"maew_cheirang_3", &CNpcScript::maew_cheirang_3},
	{"maew_cheirang_4", &CNpcScript::maew_cheirang_4},
	{"maew_cheirang_5", &CNpcScript::maew_cheirang_5},
	{"maew_cheirang_6", &CNpcScript::maew_cheirang_6},
	{"maew_cheirang_8", &CNpcScript::maew_cheirang_8},
	{"maew_cheirang_9", &CNpcScript::maew_cheirang_9},
	{"maew_cheirang_10", &CNpcScript::maew_cheirang_10},
	{"maew_karl_0", &CNpcScript::maew_karl_0},
	{"maew_karl_1", &CNpcScript::maew_karl_1},
	{"maew_karl_2", &CNpcScript::maew_karl_2},
	{"maew_karl_3", &CNpcScript::maew_karl_3},
	{"maew_karl_4", &CNpcScript::maew_karl_4},
	{"maew_karl_5", &CNpcScript::maew_karl_5},
	{"maew_karl_6", &CNpcScript::maew_karl_6},
	{"maew_karl_8", &CNpcScript::maew_karl_8},
	{"maew_karl_9", &CNpcScript::maew_karl_9},
	{"maew_karl_10", &CNpcScript::maew_karl_10},
	{"maew_isilis_0", &CNpcScript::maew_isilis_0},
	{"maew_isilis_1", &CNpcScript::maew_isilis_1},
	{"maew_isilis_2", &CNpcScript::maew_isilis_2},
	{"maew_isilis_3", &CNpcScript::maew_isilis_3},
	{"maew_isilis_4", &CNpcScript::maew_isilis_4},
	{"maew_isilis_5", &CNpcScript::maew_isilis_5},
	{"maew_isilis_6", &CNpcScript::maew_isilis_6},
	{"maew_isilis_8", &CNpcScript::maew_isilis_8},
	{"maew_isilis_9", &CNpcScript::maew_isilis_9},
	{"maew_isilis_10", &CNpcScript::maew_isilis_10},
	{"maew_branka_0", &CNpcScript::maew_branka_0},
	{"maew_branka_1", &CNpcScript::maew_branka_1},
	{"maew_branka_2", &CNpcScript::maew_branka_2},
	{"maew_branka_3", &CNpcScript::maew_branka_3},
	{"maew_branka_4", &CNpcScript::maew_branka_4},
	{"maew_branka_5", &CNpcScript::maew_branka_5},
	{"maew_branka_6", &CNpcScript::maew_branka_6},
	{"maew_branka_8", &CNpcScript::maew_branka_8},
	{"maew_branka_9", &CNpcScript::maew_branka_9},
	{"maew_branka_10", &CNpcScript::maew_branka_10},
	{"maew_romain_0", &CNpcScript::maew_romain_0},
	{"maew_romain_1", &CNpcScript::maew_romain_1},
	{"maew_romain_2", &CNpcScript::maew_romain_2},
	{"maew_romain_3", &CNpcScript::maew_romain_3},
	{"maew_romain_4", &CNpcScript::maew_romain_4},
	{"maew_romain_5", &CNpcScript::maew_romain_5},
	{"maew_romain_6", &CNpcScript::maew_romain_6},
	{"maew_romain_8", &CNpcScript::maew_romain_8},
	{"maew_romain_9", &CNpcScript::maew_romain_9},
	{"maew_romain_10", &CNpcScript::maew_romain_10},
	{"maew_huntrang_0", &CNpcScript::maew_huntrang_0},
	{"maew_huntrang_1", &CNpcScript::maew_huntrang_1},
	{"maew_huntrang_2", &CNpcScript::maew_huntrang_2},
	{"maew_huntrang_3", &CNpcScript::maew_huntrang_3},
	{"maew_huntrang_4", &CNpcScript::maew_huntrang_4},
	{"maew_huntrang_5", &CNpcScript::maew_huntrang_5},
	{"maew_huntrang_6", &CNpcScript::maew_huntrang_6},
	{"maew_huntrang_8", &CNpcScript::maew_huntrang_8},
	{"maew_huntrang_9", &CNpcScript::maew_huntrang_9},
	{"maew_huntrang_10", &CNpcScript::maew_huntrang_10},
	{"maew_miorang_0", &CNpcScript::maew_miorang_0},
	{"maew_miorang_1", &CNpcScript::maew_miorang_1},
	{"maew_miorang_2", &CNpcScript::maew_miorang_2},
	{"maew_miorang_3", &CNpcScript::maew_miorang_3},
	{"maew_miorang_4", &CNpcScript::maew_miorang_4},
	{"maew_miorang_5", &CNpcScript::maew_miorang_5},
	{"maew_miorang_6", &CNpcScript::maew_miorang_6},
	{"maew_miorang_8", &CNpcScript::maew_miorang_8},
	{"maew_miorang_9", &CNpcScript::maew_miorang_9},
	{"maew_miorang_10", &CNpcScript::maew_miorang_10},
	{"maew_kanrang_0", &CNpcScript::maew_kanrang_0},
	{"maew_kanrang_1", &CNpcScript::maew_kanrang_1},
	{"maew_kanrang_2", &CNpcScript::maew_kanrang_2},
	{"maew_kanrang_3", &CNpcScript::maew_kanrang_3},
	{"maew_kanrang_4", &CNpcScript::maew_kanrang_4},
	{"maew_kanrang_5", &CNpcScript::maew_kanrang_5},
	{"maew_kanrang_6", &CNpcScript::maew_kanrang_6},
	{"maew_kanrang_8", &CNpcScript::maew_kanrang_8},
	{"maew_kanrang_9", &CNpcScript::maew_kanrang_9},
	{"maew_kanrang_10", &CNpcScript::maew_kanrang_10},
	{"maew_karang_0", &CNpcScript::maew_karang_0},
	{"maew_karang_1", &CNpcScript::maew_karang_1},
	{"maew_karang_2", &CNpcScript::maew_karang_2},
	{"maew_karang_3", &CNpcScript::maew_karang_3},
	{"maew_karang_4", &CNpcScript::maew_karang_4},
	{"maew_karang_5", &CNpcScript::maew_karang_5},
	{"maew_karang_6", &CNpcScript::maew_karang_6},
	{"maew_karang_8", &CNpcScript::maew_karang_8},
	{"maew_karang_9", &CNpcScript::maew_karang_9},
	{"maew_karang_10", &CNpcScript::maew_karang_10},
	{"maew_kurang_0", &CNpcScript::maew_kurang_0},
	{"maew_kurang_1", &CNpcScript::maew_kurang_1},
	{"maew_kurang_2", &CNpcScript::maew_kurang_2},
	{"maew_kurang_3", &CNpcScript::maew_kurang_3},
	{"maew_kurang_4", &CNpcScript::maew_kurang_4},
	{"maew_kurang_5", &CNpcScript::maew_kurang_5},
	{"maew_kurang_6", &CNpcScript::maew_kurang_6},
	{"maew_kurang_8", &CNpcScript::maew_kurang_8},
	{"maew_kurang_9", &CNpcScript::maew_kurang_9},
	{"maew_kurang_10", &CNpcScript::maew_kurang_10},
	{"maew_mawrang_0", &CNpcScript::maew_mawrang_0},
	{"maew_mawrang_1", &CNpcScript::maew_mawrang_1},
	{"maew_mawrang_2", &CNpcScript::maew_mawrang_2},
	{"maew_mawrang_3", &CNpcScript::maew_mawrang_3},
	{"maew_mawrang_4", &CNpcScript::maew_mawrang_4},
	{"maew_mawrang_5", &CNpcScript::maew_mawrang_5},
	{"maew_mawrang_6", &CNpcScript::maew_mawrang_6},
	{"maew_mawrang_8", &CNpcScript::maew_mawrang_8},
	{"maew_mawrang_9", &CNpcScript::maew_mawrang_9},
	{"maew_mawrang_10", &CNpcScript::maew_mawrang_10},
	{"maew_maurang_0", &CNpcScript::maew_maurang_0},
	{"maew_maurang_1", &CNpcScript::maew_maurang_1},
	{"maew_maurang_2", &CNpcScript::maew_maurang_2},
	{"maew_maurang_3", &CNpcScript::maew_maurang_3},
	{"maew_maurang_4", &CNpcScript::maew_maurang_4},
	{"maew_maurang_5", &CNpcScript::maew_maurang_5},
	{"maew_maurang_6", &CNpcScript::maew_maurang_6},
	{"maew_maurang_8", &CNpcScript::maew_maurang_8},
	{"maew_maurang_9", &CNpcScript::maew_maurang_9},
	{"maew_maurang_10", &CNpcScript::maew_maurang_10},
	{"maew_arang_0", &CNpcScript::maew_arang_0},
	{"maew_arang_1", &CNpcScript::maew_arang_1},
	{"maew_arang_2", &CNpcScript::maew_arang_2},
	{"maew_arang_3", &CNpcScript::maew_arang_3},
	{"maew_arang_4", &CNpcScript::maew_arang_4},
	{"maew_arang_5", &CNpcScript::maew_arang_5},
	{"maew_arang_6", &CNpcScript::maew_arang_6},
	{"maew_arang_8", &CNpcScript::maew_arang_8},
	{"maew_arang_9", &CNpcScript::maew_arang_9},
	{"maew_arang_10", &CNpcScript::maew_arang_10},
	{"maew_bato_0", &CNpcScript::maew_bato_0},
	{"maew_bato_1", &CNpcScript::maew_bato_1},
	{"maew_bato_2", &CNpcScript::maew_bato_2},
	{"maew_bato_3", &CNpcScript::maew_bato_3},
	{"maew_bato_4", &CNpcScript::maew_bato_4},
	{"maew_bato_5", &CNpcScript::maew_bato_5},
	{"maew_bato_6", &CNpcScript::maew_bato_6},
	{"maew_bato_8", &CNpcScript::maew_bato_8},
	{"maew_bato_9", &CNpcScript::maew_bato_9},
	{"maew_bato_10", &CNpcScript::maew_bato_10},
	{"maew_meialune_0", &CNpcScript::maew_meialune_0},
	{"maew_meialune_1", &CNpcScript::maew_meialune_1},
	{"maew_meialune_2", &CNpcScript::maew_meialune_2},
	{"maew_meialune_3", &CNpcScript::maew_meialune_3},
	{"maew_meialune_4", &CNpcScript::maew_meialune_4},
	{"maew_meialune_5", &CNpcScript::maew_meialune_5},
	{"maew_meialune_6", &CNpcScript::maew_meialune_6},
	{"maew_meialune_8", &CNpcScript::maew_meialune_8},
	{"maew_meialune_9", &CNpcScript::maew_meialune_9},
	{"maew_meialune_10", &CNpcScript::maew_meialune_10},
	{"maew_hawrang_0", &CNpcScript::maew_hawrang_0},
	{"maew_hawrang_1", &CNpcScript::maew_hawrang_1},
	{"maew_hawrang_2", &CNpcScript::maew_hawrang_2},
	{"maew_hawrang_3", &CNpcScript::maew_hawrang_3},
	{"maew_hawrang_4", &CNpcScript::maew_hawrang_4},
	{"maew_hawrang_5", &CNpcScript::maew_hawrang_5},
	{"maew_hawrang_6", &CNpcScript::maew_hawrang_6},
	{"maew_hawrang_8", &CNpcScript::maew_hawrang_8},
	{"maew_hawrang_9", &CNpcScript::maew_hawrang_9},
	{"maew_hawrang_10", &CNpcScript::maew_hawrang_10},
	{"maew_heirang_0", &CNpcScript::maew_heirang_0},
	{"maew_heirang_1", &CNpcScript::maew_heirang_1},
	{"maew_heirang_2", &CNpcScript::maew_heirang_2},
	{"maew_heirang_3", &CNpcScript::maew_heirang_3},
	{"maew_heirang_4", &CNpcScript::maew_heirang_4},
	{"maew_heirang_5", &CNpcScript::maew_heirang_5},
	{"maew_heirang_6", &CNpcScript::maew_heirang_6},
	{"maew_heirang_8", &CNpcScript::maew_heirang_8},
	{"maew_heirang_9", &CNpcScript::maew_heirang_9},
	{"maew_heirang_10", &CNpcScript::maew_heirang_10},
	{"maew_ralbadan_0", &CNpcScript::maew_ralbadan_0},
	{"maew_ralbadan_1", &CNpcScript::maew_ralbadan_1},
	{"maew_ralbadan_2", &CNpcScript::maew_ralbadan_2},
	{"maew_ralbadan_3", &CNpcScript::maew_ralbadan_3},
	{"maew_ralbadan_4", &CNpcScript::maew_ralbadan_4},
	{"maew_ralbadan_5", &CNpcScript::maew_ralbadan_5},
	{"maew_ralbadan_6", &CNpcScript::maew_ralbadan_6},
	{"maew_ralbadan_8", &CNpcScript::maew_ralbadan_8},
	{"maew_ralbadan_9", &CNpcScript::maew_ralbadan_9},
	{"maew_ralbadan_10", &CNpcScript::maew_ralbadan_10},
	{"maew_rutam_0", &CNpcScript::maew_rutam_0},
	{"maew_rutam_1", &CNpcScript::maew_rutam_1},
	{"maew_rutam_2", &CNpcScript::maew_rutam_2},
	{"maew_rutam_3", &CNpcScript::maew_rutam_3},
	{"maew_rutam_4", &CNpcScript::maew_rutam_4},
	{"maew_rutam_5", &CNpcScript::maew_rutam_5},
	{"maew_rutam_6", &CNpcScript::maew_rutam_6},
	{"maew_rutam_8", &CNpcScript::maew_rutam_8},
	{"maew_rutam_9", &CNpcScript::maew_rutam_9},
	{"maew_rutam_10", &CNpcScript::maew_rutam_10},
	{"maew_raya_0", &CNpcScript::maew_raya_0},
	{"maew_raya_1", &CNpcScript::maew_raya_1},
	{"maew_raya_2", &CNpcScript::maew_raya_2},
	{"maew_raya_3", &CNpcScript::maew_raya_3},
	{"maew_raya_4", &CNpcScript::maew_raya_4},
	{"maew_raya_5", &CNpcScript::maew_raya_5},
	{"maew_raya_6", &CNpcScript::maew_raya_6},
	{"maew_raya_8", &CNpcScript::maew_raya_8},
	{"maew_raya_9", &CNpcScript::maew_raya_9},
	{"maew_raya_10", &CNpcScript::maew_raya_10},
	{"maew_harlie_0", &CNpcScript::maew_harlie_0},
	{"maew_harlie_1", &CNpcScript::maew_harlie_1},
	{"maew_harlie_2", &CNpcScript::maew_harlie_2},
	{"maew_harlie_3", &CNpcScript::maew_harlie_3},
	{"maew_harlie_4", &CNpcScript::maew_harlie_4},
	{"maew_harlie_5", &CNpcScript::maew_harlie_5},
	{"maew_harlie_6", &CNpcScript::maew_harlie_6},
	{"maew_harlie_8", &CNpcScript::maew_harlie_8},
	{"maew_harlie_9", &CNpcScript::maew_harlie_9},
	{"maew_harlie_10", &CNpcScript::maew_harlie_10},
	{"maew_rodella_0", &CNpcScript::maew_rodella_0},
	{"maew_rodella_1", &CNpcScript::maew_rodella_1},
	{"maew_rodella_2", &CNpcScript::maew_rodella_2},
	{"maew_rodella_3", &CNpcScript::maew_rodella_3},
	{"maew_rodella_4", &CNpcScript::maew_rodella_4},
	{"maew_rodella_5", &CNpcScript::maew_rodella_5},
	{"maew_rodella_6", &CNpcScript::maew_rodella_6},
	{"maew_rodella_8", &CNpcScript::maew_rodella_8},
	{"maew_rodella_9", &CNpcScript::maew_rodella_9},
	{"maew_rodella_10", &CNpcScript::maew_rodella_10},
	{"maew_gelga_0", &CNpcScript::maew_gelga_0},
	{"maew_gelga_1", &CNpcScript::maew_gelga_1},
	{"maew_gelga_2", &CNpcScript::maew_gelga_2},
	{"maew_gelga_3", &CNpcScript::maew_gelga_3},
	{"maew_gelga_4", &CNpcScript::maew_gelga_4},
	{"maew_gelga_5", &CNpcScript::maew_gelga_5},
	{"maew_gelga_6", &CNpcScript::maew_gelga_6},
	{"maew_gelga_8", &CNpcScript::maew_gelga_8},
	{"maew_gelga_9", &CNpcScript::maew_gelga_9},
	{"maew_gelga_10", &CNpcScript::maew_gelga_10},
	{"maew_huri_0", &CNpcScript::maew_huri_0},
	{"maew_huri_1", &CNpcScript::maew_huri_1},
	{"maew_huri_2", &CNpcScript::maew_huri_2},
	{"maew_huri_3", &CNpcScript::maew_huri_3},
	{"maew_huri_4", &CNpcScript::maew_huri_4},
	{"maew_huri_5", &CNpcScript::maew_huri_5},
	{"maew_huri_6", &CNpcScript::maew_huri_6},
	{"maew_huri_8", &CNpcScript::maew_huri_8},
	{"maew_huri_9", &CNpcScript::maew_huri_9},
	{"maew_huri_10", &CNpcScript::maew_huri_10},
	{"maew_epie_0", &CNpcScript::maew_epie_0},
	{"maew_epie_1", &CNpcScript::maew_epie_1},
	{"maew_epie_2", &CNpcScript::maew_epie_2},
	{"maew_epie_3", &CNpcScript::maew_epie_3},
	{"maew_epie_4", &CNpcScript::maew_epie_4},
	{"maew_epie_5", &CNpcScript::maew_epie_5},
	{"maew_epie_6", &CNpcScript::maew_epie_6},
	{"maew_epie_8", &CNpcScript::maew_epie_8},
	{"maew_epie_9", &CNpcScript::maew_epie_9},
	{"maew_epie_10", &CNpcScript::maew_epie_10},
	{"maew_ruoboru_0", &CNpcScript::maew_ruoboru_0},
	{"maew_ruoboru_1", &CNpcScript::maew_ruoboru_1},
	{"maew_ruoboru_2", &CNpcScript::maew_ruoboru_2},
	{"maew_ruoboru_3", &CNpcScript::maew_ruoboru_3},
	{"maew_ruoboru_4", &CNpcScript::maew_ruoboru_4},
	{"maew_ruoboru_5", &CNpcScript::maew_ruoboru_5},
	{"maew_ruoboru_6", &CNpcScript::maew_ruoboru_6},
	{"maew_ruoboru_8", &CNpcScript::maew_ruoboru_8},
	{"maew_ruoboru_9", &CNpcScript::maew_ruoboru_9},
	{"maew_ruoboru_10", &CNpcScript::maew_ruoboru_10},
	{"maew_ruidan_0", &CNpcScript::maew_ruidan_0},
	{"maew_ruidan_1", &CNpcScript::maew_ruidan_1},
	{"maew_ruidan_2", &CNpcScript::maew_ruidan_2},
	{"maew_ruidan_3", &CNpcScript::maew_ruidan_3},
	{"maew_ruidan_4", &CNpcScript::maew_ruidan_4},
	{"maew_ruidan_5", &CNpcScript::maew_ruidan_5},
	{"maew_ruidan_6", &CNpcScript::maew_ruidan_6},
	{"maew_ruidan_8", &CNpcScript::maew_ruidan_8},
	{"maew_ruidan_9", &CNpcScript::maew_ruidan_9},
	{"maew_ruidan_10", &CNpcScript::maew_ruidan_10},
	{"maew_relgantus_0", &CNpcScript::maew_relgantus_0},
	{"maew_relgantus_1", &CNpcScript::maew_relgantus_1},
	{"maew_relgantus_2", &CNpcScript::maew_relgantus_2},
	{"maew_relgantus_3", &CNpcScript::maew_relgantus_3},
	{"maew_relgantus_4", &CNpcScript::maew_relgantus_4},
	{"maew_relgantus_5", &CNpcScript::maew_relgantus_5},
	{"maew_relgantus_6", &CNpcScript::maew_relgantus_6},
	{"maew_relgantus_8", &CNpcScript::maew_relgantus_8},
	{"maew_relgantus_9", &CNpcScript::maew_relgantus_9},
	{"maew_relgantus_10", &CNpcScript::maew_relgantus_10},
	{"maew_miriun_0", &CNpcScript::maew_miriun_0},
	{"maew_miriun_1", &CNpcScript::maew_miriun_1},
	{"maew_miriun_2", &CNpcScript::maew_miriun_2},
	{"maew_miriun_3", &CNpcScript::maew_miriun_3},
	{"maew_miriun_4", &CNpcScript::maew_miriun_4},
	{"maew_miriun_5", &CNpcScript::maew_miriun_5},
	{"maew_miriun_6", &CNpcScript::maew_miriun_6},
	{"maew_miriun_8", &CNpcScript::maew_miriun_8},
	{"maew_miriun_9", &CNpcScript::maew_miriun_9},
	{"maew_miriun_10", &CNpcScript::maew_miriun_10},
	{"maew_emblum_0", &CNpcScript::maew_emblum_0},
	{"maew_emblum_1", &CNpcScript::maew_emblum_1},
	{"maew_emblum_2", &CNpcScript::maew_emblum_2},
	{"maew_emblum_3", &CNpcScript::maew_emblum_3},
	{"maew_emblum_4", &CNpcScript::maew_emblum_4},
	{"maew_emblum_5", &CNpcScript::maew_emblum_5},
	{"maew_emblum_6", &CNpcScript::maew_emblum_6},
	{"maew_emblum_8", &CNpcScript::maew_emblum_8},
	{"maew_emblum_9", &CNpcScript::maew_emblum_9},
	{"maew_emblum_10", &CNpcScript::maew_emblum_10},
	{"maew_mewrang_0", &CNpcScript::maew_mewrang_0},
	{"maew_mewrang_1", &CNpcScript::maew_mewrang_1},
	{"maew_mewrang_2", &CNpcScript::maew_mewrang_2},
	{"maew_mewrang_3", &CNpcScript::maew_mewrang_3},
	{"maew_mewrang_4", &CNpcScript::maew_mewrang_4},
	{"maew_mewrang_5", &CNpcScript::maew_mewrang_5},
	{"maew_mewrang_6", &CNpcScript::maew_mewrang_6},
	{"maew_mewrang_8", &CNpcScript::maew_mewrang_8},
	{"maew_mewrang_9", &CNpcScript::maew_mewrang_9},
	{"maew_mewrang_10", &CNpcScript::maew_mewrang_10},
	{"mada_dark_0", &CNpcScript::mada_dark_0},
	{"mada_dark_1", &CNpcScript::mada_dark_1},
	{"mada_dark_2", &CNpcScript::mada_dark_2},
	{"mada_dark_3", &CNpcScript::mada_dark_3},
	{"mada_dark_4", &CNpcScript::mada_dark_4},
	{"mada_dark_5", &CNpcScript::mada_dark_5},
	{"mada_dark_6", &CNpcScript::mada_dark_6},
	{"mada_dark_8", &CNpcScript::mada_dark_8},
	{"mada_dark_9", &CNpcScript::mada_dark_9},
	{"mada_dark_10", &CNpcScript::mada_dark_10},
	{"masa_light_0", &CNpcScript::masa_light_0},
	{"masa_light_1", &CNpcScript::masa_light_1},
	{"masa_light_2", &CNpcScript::masa_light_2},
	{"masa_light_3", &CNpcScript::masa_light_3},
	{"masa_light_4", &CNpcScript::masa_light_4},
	{"masa_light_5", &CNpcScript::masa_light_5},
	{"masa_light_6", &CNpcScript::masa_light_6},
	{"masa_light_8", &CNpcScript::masa_light_8},
	{"masa_light_9", &CNpcScript::masa_light_9},
	{"masa_light_10", &CNpcScript::masa_light_10},
	{"mafl_romero_0", &CNpcScript::mafl_romero_0},
	{"mafl_romero_1", &CNpcScript::mafl_romero_1},
	{"mafl_romero_2", &CNpcScript::mafl_romero_2},
	{"mafl_romero_3", &CNpcScript::mafl_romero_3},
	{"mafl_romero_4", &CNpcScript::mafl_romero_4},
	{"mafl_romero_5", &CNpcScript::mafl_romero_5},
	{"mafl_romero_6", &CNpcScript::mafl_romero_6},
	{"mafl_romero_8", &CNpcScript::mafl_romero_8},
	{"mafl_romero_9", &CNpcScript::mafl_romero_9},
	{"mafl_romero_10", &CNpcScript::mafl_romero_10},
	{"mafl_frankenstein_0", &CNpcScript::mafl_frankenstein_0},
	{"mafl_frankenstein_1", &CNpcScript::mafl_frankenstein_1},
	{"mafl_frankenstein_2", &CNpcScript::mafl_frankenstein_2},
	{"mafl_frankenstein_3", &CNpcScript::mafl_frankenstein_3},
	{"mafl_frankenstein_4", &CNpcScript::mafl_frankenstein_4},
	{"mafl_frankenstein_5", &CNpcScript::mafl_frankenstein_5},
	{"mafl_frankenstein_6", &CNpcScript::mafl_frankenstein_6},
	{"mafl_frankenstein_8", &CNpcScript::mafl_frankenstein_8},
	{"mafl_frankenstein_9", &CNpcScript::mafl_frankenstein_9},
	{"mafl_frankenstein_10", &CNpcScript::mafl_frankenstein_10},
	{"masa_roa_0", &CNpcScript::masa_roa_0},
	{"masa_roa_1", &CNpcScript::masa_roa_1},
	{"masa_roa_2", &CNpcScript::masa_roa_2},
	{"masa_roa_3", &CNpcScript::masa_roa_3},
	{"masa_roa_4", &CNpcScript::masa_roa_4},
	{"masa_roa_5", &CNpcScript::masa_roa_5},
	{"masa_roa_6", &CNpcScript::masa_roa_6},
	{"masa_roa_8", &CNpcScript::masa_roa_8},
	{"masa_roa_9", &CNpcScript::masa_roa_9},
	{"masa_roa_10", &CNpcScript::masa_roa_10},
	{"maew_troy_0", &CNpcScript::maew_troy_0},
	{"maew_troy_1", &CNpcScript::maew_troy_1},
	{"maew_troy_2", &CNpcScript::maew_troy_2},
	{"maew_troy_3", &CNpcScript::maew_troy_3},
	{"maew_troy_4", &CNpcScript::maew_troy_4},
	{"maew_troy_5", &CNpcScript::maew_troy_5},
	{"maew_troy_6", &CNpcScript::maew_troy_6},
	{"maew_troy_8", &CNpcScript::maew_troy_8},
	{"maew_troy_9", &CNpcScript::maew_troy_9},
	{"maew_troy_10", &CNpcScript::maew_troy_10},
	{"mafl_brooks_0", &CNpcScript::mafl_brooks_0},
	{"mafl_brooks_1", &CNpcScript::mafl_brooks_1},
	{"mafl_brooks_2", &CNpcScript::mafl_brooks_2},
	{"mafl_brooks_3", &CNpcScript::mafl_brooks_3},
	{"mafl_brooks_4", &CNpcScript::mafl_brooks_4},
	{"mafl_brooks_5", &CNpcScript::mafl_brooks_5},
	{"mafl_brooks_6", &CNpcScript::mafl_brooks_6},
	{"mafl_brooks_8", &CNpcScript::mafl_brooks_8},
	{"mafl_brooks_9", &CNpcScript::mafl_brooks_9},
	{"mafl_brooks_10", &CNpcScript::mafl_brooks_10},	
	{"mafl_demian_0", &CNpcScript::mafl_demian_0},
	{"mafl_demian_1", &CNpcScript::mafl_demian_1},
	{"mafl_demian_2", &CNpcScript::mafl_demian_2},
	{"mafl_demian_3", &CNpcScript::mafl_demian_3},
	{"mafl_demian_4", &CNpcScript::mafl_demian_4},
	{"mafl_demian_5", &CNpcScript::mafl_demian_5},
	{"mafl_demian_6", &CNpcScript::mafl_demian_6},
	{"mafl_demian_8", &CNpcScript::mafl_demian_8},
	{"mafl_demian_9", &CNpcScript::mafl_demian_9},
	{"mafl_demian_10", &CNpcScript::mafl_demian_10},
	{"mafl_atma_0", &CNpcScript::mafl_atma_0},
	{"mafl_atma_1", &CNpcScript::mafl_atma_1},
	{"mafl_atma_2", &CNpcScript::mafl_atma_2},
	{"mafl_atma_3", &CNpcScript::mafl_atma_3},
	{"mafl_atma_4", &CNpcScript::mafl_atma_4},
	{"mafl_atma_5", &CNpcScript::mafl_atma_5},
	{"mafl_atma_6", &CNpcScript::mafl_atma_6},
	{"mafl_atma_8", &CNpcScript::mafl_atma_8},
	{"mafl_atma_9", &CNpcScript::mafl_atma_9},
	{"mafl_atma_10", &CNpcScript::mafl_atma_10},
	{"mafl_chance_0", &CNpcScript::mafl_chance_0},
	{"mafl_chance_1", &CNpcScript::mafl_chance_1},
	{"mafl_chance_2", &CNpcScript::mafl_chance_2},
	{"mafl_chance_3", &CNpcScript::mafl_chance_3},
	{"mafl_chance_4", &CNpcScript::mafl_chance_4},
	{"mafl_chance_5", &CNpcScript::mafl_chance_5},
	{"mafl_chance_6", &CNpcScript::mafl_chance_6},
	{"mafl_chance_8", &CNpcScript::mafl_chance_8},
	{"mafl_chance_9", &CNpcScript::mafl_chance_9},
	{"mafl_chance_10", &CNpcScript::mafl_chance_10},
	{"masa_jacklin_pott_0", &CNpcScript::masa_jacklin_pott_0},
	{"masa_jacklin_pott_1", &CNpcScript::masa_jacklin_pott_1},
	{"masa_jacklin_pott_2", &CNpcScript::masa_jacklin_pott_2},
	{"masa_jacklin_pott_3", &CNpcScript::masa_jacklin_pott_3},
	{"masa_jacklin_pott_4", &CNpcScript::masa_jacklin_pott_4},
	{"masa_jacklin_pott_5", &CNpcScript::masa_jacklin_pott_5},
	{"masa_jacklin_pott_6", &CNpcScript::masa_jacklin_pott_6},
	{"masa_jacklin_pott_8", &CNpcScript::masa_jacklin_pott_8},
	{"masa_jacklin_pott_9", &CNpcScript::masa_jacklin_pott_9},
	{"masa_jacklin_pott_10", &CNpcScript::masa_jacklin_pott_10},
	{"maew_rukas_0", &CNpcScript::maew_rukas_0},
	{"maew_rukas_1", &CNpcScript::maew_rukas_1},
	{"maew_rukas_2", &CNpcScript::maew_rukas_2},
	{"maew_rukas_3", &CNpcScript::maew_rukas_3},
	{"maew_rukas_4", &CNpcScript::maew_rukas_4},
	{"maew_rukas_5", &CNpcScript::maew_rukas_5},
	{"maew_rukas_6", &CNpcScript::maew_rukas_6},
	{"maew_rukas_8", &CNpcScript::maew_rukas_8},
	{"maew_rukas_9", &CNpcScript::maew_rukas_9},
	{"maew_rukas_10", &CNpcScript::maew_rukas_10},
	{"maew_raundas_0", &CNpcScript::maew_raundas_0},
	{"maew_raundas_1", &CNpcScript::maew_raundas_1},
	{"maew_raundas_2", &CNpcScript::maew_raundas_2},
	{"maew_raundas_3", &CNpcScript::maew_raundas_3},
	{"maew_raundas_4", &CNpcScript::maew_raundas_4},
	{"maew_raundas_5", &CNpcScript::maew_raundas_5},
	{"maew_raundas_6", &CNpcScript::maew_raundas_6},
	{"maew_raundas_8", &CNpcScript::maew_raundas_8},
	{"maew_raundas_9", &CNpcScript::maew_raundas_9},
	{"maew_raundas_10", &CNpcScript::maew_raundas_10},
	{"maew_pinanoco_0", &CNpcScript::maew_pinanoco_0},
	{"maew_pinanoco_1", &CNpcScript::maew_pinanoco_1},
	{"maew_pinanoco_2", &CNpcScript::maew_pinanoco_2},
	{"maew_pinanoco_3", &CNpcScript::maew_pinanoco_3},
	{"maew_pinanoco_4", &CNpcScript::maew_pinanoco_4},
	{"maew_pinanoco_5", &CNpcScript::maew_pinanoco_5},
	{"maew_pinanoco_6", &CNpcScript::maew_pinanoco_6},
	{"maew_pinanoco_8", &CNpcScript::maew_pinanoco_8},
	{"maew_pinanoco_9", &CNpcScript::maew_pinanoco_9},
	{"maew_pinanoco_10", &CNpcScript::maew_pinanoco_10},	
	{"mafl_kawibawibo04_0", &CNpcScript::mafl_kawibawibo04_0},
	{"mafl_kawibawibo04_1", &CNpcScript::mafl_kawibawibo04_1},
	{"mafl_kawibawibo04_2", &CNpcScript::mafl_kawibawibo04_2},
	{"mafl_kawibawibo04_3", &CNpcScript::mafl_kawibawibo04_3},
	{"mafl_kawibawibo04_4", &CNpcScript::mafl_kawibawibo04_4},
	{"mafl_kawibawibo04_5", &CNpcScript::mafl_kawibawibo04_5},
	{"mafl_kawibawibo04_6", &CNpcScript::mafl_kawibawibo04_6},
	{"mafl_kawibawibo04_8", &CNpcScript::mafl_kawibawibo04_8},
	{"mafl_kawibawibo04_9", &CNpcScript::mafl_kawibawibo04_9},
	{"mafl_kawibawibo04_10", &CNpcScript::mafl_kawibawibo04_10},	
	{"maew_idelruna_0", &CNpcScript::maew_idelruna_0},
	{"maew_idelruna_1", &CNpcScript::maew_idelruna_1},
	{"maew_idelruna_2", &CNpcScript::maew_idelruna_2},
	{"maew_idelruna_3", &CNpcScript::maew_idelruna_3},
	{"maew_idelruna_4", &CNpcScript::maew_idelruna_4},
	{"maew_idelruna_5", &CNpcScript::maew_idelruna_5},
	{"maew_idelruna_6", &CNpcScript::maew_idelruna_6},
	{"maew_idelruna_8", &CNpcScript::maew_idelruna_8},
	{"maew_idelruna_9", &CNpcScript::maew_idelruna_9},
	{"maew_idelruna_10", &CNpcScript::maew_idelruna_10},
	{"maew_gigaka_0", &CNpcScript::maew_gigaka_0},
	{"maew_gigaka_1", &CNpcScript::maew_gigaka_1},
	{"maew_gigaka_2", &CNpcScript::maew_gigaka_2},
	{"maew_gigaka_3", &CNpcScript::maew_gigaka_3},
	{"maew_gigaka_4", &CNpcScript::maew_gigaka_4},
	{"maew_gigaka_5", &CNpcScript::maew_gigaka_5},
	{"maew_gigaka_6", &CNpcScript::maew_gigaka_6},
	{"maew_gigaka_8", &CNpcScript::maew_gigaka_8},
	{"maew_gigaka_9", &CNpcScript::maew_gigaka_9},
	{"maew_gigaka_10", &CNpcScript::maew_gigaka_10},
	{"masa_christmasrubi_0", &CNpcScript::masa_christmasrubi_0},
	{"masa_christmasrubi_1", &CNpcScript::masa_christmasrubi_1},
	{"masa_christmasrubi_2", &CNpcScript::masa_christmasrubi_2},
	{"masa_christmasrubi_3", &CNpcScript::masa_christmasrubi_3},
	{"masa_christmasrubi_4", &CNpcScript::masa_christmasrubi_4},
	{"masa_christmasrubi_5", &CNpcScript::masa_christmasrubi_5},
	{"masa_christmasrubi_6", &CNpcScript::masa_christmasrubi_6},
	{"masa_christmasrubi_8", &CNpcScript::masa_christmasrubi_8},
	{"masa_christmasrubi_9", &CNpcScript::masa_christmasrubi_9},
	{"masa_christmasrubi_10", &CNpcScript::masa_christmasrubi_10},
	{"mada_christmassnow_0", &CNpcScript::mada_christmassnow_0},
	{"mada_christmassnow_1", &CNpcScript::mada_christmassnow_1},
	{"mada_christmassnow_2", &CNpcScript::mada_christmassnow_2},
	{"mada_christmassnow_3", &CNpcScript::mada_christmassnow_3},
	{"mada_christmassnow_4", &CNpcScript::mada_christmassnow_4},
	{"mada_christmassnow_5", &CNpcScript::mada_christmassnow_5},
	{"mada_christmassnow_6", &CNpcScript::mada_christmassnow_6},
	{"mada_christmassnow_8", &CNpcScript::mada_christmassnow_8},
	{"mada_christmassnow_9", &CNpcScript::mada_christmassnow_9},
	{"mada_christmassnow_10", &CNpcScript::mada_christmassnow_10},
	{"mafl_zandark_0", &CNpcScript::mafl_zandark_0},
	{"mafl_zandark_1", &CNpcScript::mafl_zandark_1},
	{"mafl_zandark_2", &CNpcScript::mafl_zandark_2},
	{"mafl_zandark_3", &CNpcScript::mafl_zandark_3},
	{"mafl_zandark_4", &CNpcScript::mafl_zandark_4},
	{"mafl_zandark_5", &CNpcScript::mafl_zandark_5},
	{"mafl_zandark_6", &CNpcScript::mafl_zandark_6},
	{"mafl_zandark_8", &CNpcScript::mafl_zandark_8},
	{"mafl_zandark_9", &CNpcScript::mafl_zandark_9},
	{"mafl_zandark_10", &CNpcScript::mafl_zandark_10},
	{"mafl_asis_0", &CNpcScript::mafl_asis_0},
	{"mafl_asis_1", &CNpcScript::mafl_asis_1},
	{"mafl_asis_2", &CNpcScript::mafl_asis_2},
	{"mafl_asis_3", &CNpcScript::mafl_asis_3},
	{"mafl_asis_4", &CNpcScript::mafl_asis_4},
	{"mafl_asis_5", &CNpcScript::mafl_asis_5},
	{"mafl_asis_6", &CNpcScript::mafl_asis_6},
	{"mafl_asis_8", &CNpcScript::mafl_asis_8},
	{"mafl_asis_9", &CNpcScript::mafl_asis_9},
	{"mafl_asis_10", &CNpcScript::mafl_asis_10},
	{"mafl_ssunder_0", &CNpcScript::mafl_ssunder_0},
	{"mafl_ssunder_1", &CNpcScript::mafl_ssunder_1},
	{"mafl_ssunder_2", &CNpcScript::mafl_ssunder_2},
	{"mafl_ssunder_3", &CNpcScript::mafl_ssunder_3},
	{"mafl_ssunder_4", &CNpcScript::mafl_ssunder_4},
	{"mafl_ssunder_5", &CNpcScript::mafl_ssunder_5},
	{"mafl_ssunder_6", &CNpcScript::mafl_ssunder_6},
	{"mafl_ssunder_8", &CNpcScript::mafl_ssunder_8},
	{"mafl_ssunder_9", &CNpcScript::mafl_ssunder_9},
	{"mafl_ssunder_10", &CNpcScript::mafl_ssunder_10},
	{"mafl_masics_0", &CNpcScript::mafl_masics_0},
	{"mafl_masics_1", &CNpcScript::mafl_masics_1},
	{"mafl_masics_2", &CNpcScript::mafl_masics_2},
	{"mafl_masics_3", &CNpcScript::mafl_masics_3},
	{"mafl_masics_4", &CNpcScript::mafl_masics_4},
	{"mafl_masics_5", &CNpcScript::mafl_masics_5},
	{"mafl_masics_6", &CNpcScript::mafl_masics_6},
	{"mafl_masics_8", &CNpcScript::mafl_masics_8},
	{"mafl_masics_9", &CNpcScript::mafl_masics_9},
	{"mafl_masics_10", &CNpcScript::mafl_masics_10},
	{"mafl_ringda_0", &CNpcScript::mafl_ringda_0},
	{"mafl_ringda_1", &CNpcScript::mafl_ringda_1},
	{"mafl_ringda_2", &CNpcScript::mafl_ringda_2},
	{"mafl_ringda_3", &CNpcScript::mafl_ringda_3},
	{"mafl_ringda_4", &CNpcScript::mafl_ringda_4},
	{"mafl_ringda_5", &CNpcScript::mafl_ringda_5},
	{"mafl_ringda_6", &CNpcScript::mafl_ringda_6},
	{"mafl_ringda_8", &CNpcScript::mafl_ringda_8},
	{"mafl_ringda_9", &CNpcScript::mafl_ringda_9},
	{"mafl_ringda_10", &CNpcScript::mafl_ringda_10},
	{"mafl_ria_0", &CNpcScript::mafl_ria_0},
	{"mafl_ria_1", &CNpcScript::mafl_ria_1},
	{"mafl_ria_2", &CNpcScript::mafl_ria_2},
	{"mafl_ria_3", &CNpcScript::mafl_ria_3},
	{"mafl_ria_4", &CNpcScript::mafl_ria_4},
	{"mafl_ria_5", &CNpcScript::mafl_ria_5},
	{"mafl_ria_6", &CNpcScript::mafl_ria_6},
	{"mafl_ria_8", &CNpcScript::mafl_ria_8},
	{"mafl_ria_9", &CNpcScript::mafl_ria_9},
	{"mafl_ria_10", &CNpcScript::mafl_ria_10},
	{"mafl_ruks_0", &CNpcScript::mafl_ruks_0},
	{"mafl_ruks_1", &CNpcScript::mafl_ruks_1},
	{"mafl_ruks_2", &CNpcScript::mafl_ruks_2},
	{"mafl_ruks_3", &CNpcScript::mafl_ruks_3},
	{"mafl_ruks_4", &CNpcScript::mafl_ruks_4},
	{"mafl_ruks_5", &CNpcScript::mafl_ruks_5},
	{"mafl_ruks_6", &CNpcScript::mafl_ruks_6},
	{"mafl_ruks_8", &CNpcScript::mafl_ruks_8},
	{"mafl_ruks_9", &CNpcScript::mafl_ruks_9},
	{"mafl_ruks_10", &CNpcScript::mafl_ruks_10},
	{"mafl_pire_0", &CNpcScript::mafl_pire_0},
	{"mafl_pire_1", &CNpcScript::mafl_pire_1},
	{"mafl_pire_2", &CNpcScript::mafl_pire_2},
	{"mafl_pire_3", &CNpcScript::mafl_pire_3},
	{"mafl_pire_4", &CNpcScript::mafl_pire_4},
	{"mafl_pire_5", &CNpcScript::mafl_pire_5},
	{"mafl_pire_6", &CNpcScript::mafl_pire_6},
	{"mafl_pire_8", &CNpcScript::mafl_pire_8},
	{"mafl_pire_9", &CNpcScript::mafl_pire_9},
	{"mafl_pire_10", &CNpcScript::mafl_pire_10},
	{"npc_flyffkimpd_0", &CNpcScript::npc_flyffkimpd_0},
	{"npc_flyffkimpd_1", &CNpcScript::npc_flyffkimpd_1},
	{"npc_flyffkimpd_2", &CNpcScript::npc_flyffkimpd_2},
	{"npc_flyffkimpd_3", &CNpcScript::npc_flyffkimpd_3},
	{"npc_flyffkimpd_4", &CNpcScript::npc_flyffkimpd_4},
	{"npc_flyffkimpd_5", &CNpcScript::npc_flyffkimpd_5},
	{"npc_flyffkimpd_6", &CNpcScript::npc_flyffkimpd_6},
	{"npc_flyffkimpd_8", &CNpcScript::npc_flyffkimpd_8},
	{"npc_flyffkimpd_9", &CNpcScript::npc_flyffkimpd_9},
	{"npc_flyffkimpd_10", &CNpcScript::npc_flyffkimpd_10},
	{"mafl_colonpc_0", &CNpcScript::mafl_colonpc_0},
	{"mafl_colonpc_1", &CNpcScript::mafl_colonpc_1},
	{"mafl_colonpc_2", &CNpcScript::mafl_colonpc_2},
	{"mafl_colonpc_3", &CNpcScript::mafl_colonpc_3},
	{"mafl_colonpc_4", &CNpcScript::mafl_colonpc_4},
	{"mafl_colonpc_5", &CNpcScript::mafl_colonpc_5},
	{"mafl_colonpc_6", &CNpcScript::mafl_colonpc_6},
	{"mafl_colonpc_8", &CNpcScript::mafl_colonpc_8},
	{"mafl_colonpc_9", &CNpcScript::mafl_colonpc_9},
	{"mafl_colonpc_10", &CNpcScript::mafl_colonpc_10},
	{"mafl_angel_0", &CNpcScript::mafl_angel_0},
	{"mafl_angel_1", &CNpcScript::mafl_angel_1},
	{"mafl_angel_2", &CNpcScript::mafl_angel_2},
	{"mafl_angel_3", &CNpcScript::mafl_angel_3},
	{"mafl_angel_4", &CNpcScript::mafl_angel_4},
	{"mafl_angel_5", &CNpcScript::mafl_angel_5},
	{"mafl_angel_6", &CNpcScript::mafl_angel_6},
	{"mafl_angel_8", &CNpcScript::mafl_angel_8},
	{"mafl_angel_9", &CNpcScript::mafl_angel_9},
	{"mafl_angel_10", &CNpcScript::mafl_angel_10},
	{"mafl_devil_0", &CNpcScript::mafl_devil_0},
	{"mafl_devil_1", &CNpcScript::mafl_devil_1},
	{"mafl_devil_2", &CNpcScript::mafl_devil_2},
	{"mafl_devil_3", &CNpcScript::mafl_devil_3},
	{"mafl_devil_4", &CNpcScript::mafl_devil_4},
	{"mafl_devil_5", &CNpcScript::mafl_devil_5},
	{"mafl_devil_6", &CNpcScript::mafl_devil_6},
	{"mafl_devil_8", &CNpcScript::mafl_devil_8},
	{"mafl_devil_9", &CNpcScript::mafl_devil_9},
	{"mafl_devil_10", &CNpcScript::mafl_devil_10},
	{"mafl_bongbo_0", &CNpcScript::mafl_bongbo_0},
	{"mafl_bongbo_1", &CNpcScript::mafl_bongbo_1},
	{"mafl_bongbo_2", &CNpcScript::mafl_bongbo_2},
	{"mafl_bongbo_3", &CNpcScript::mafl_bongbo_3},
	{"mafl_bongbo_4", &CNpcScript::mafl_bongbo_4},
	{"mafl_bongbo_5", &CNpcScript::mafl_bongbo_5},
	{"mafl_bongbo_6", &CNpcScript::mafl_bongbo_6},
	{"mafl_bongbo_8", &CNpcScript::mafl_bongbo_8},
	{"mafl_bongbo_9", &CNpcScript::mafl_bongbo_9},
	{"mafl_bongbo_10", &CNpcScript::mafl_bongbo_10},
	{"mafl_stake_0", &CNpcScript::mafl_stake_0},
	{"mafl_stake_1", &CNpcScript::mafl_stake_1},
	{"mafl_stake_2", &CNpcScript::mafl_stake_2},
	{"mafl_stake_3", &CNpcScript::mafl_stake_3},
	{"mafl_stake_4", &CNpcScript::mafl_stake_4},
	{"mafl_stake_5", &CNpcScript::mafl_stake_5},
	{"mafl_stake_6", &CNpcScript::mafl_stake_6},
	{"mafl_stake_8", &CNpcScript::mafl_stake_8},
	{"mafl_stake_9", &CNpcScript::mafl_stake_9},
	{"mafl_stake_10", &CNpcScript::mafl_stake_10},
	{"mafl_jakgui_0", &CNpcScript::mafl_jakgui_0},
	{"mafl_jakgui_1", &CNpcScript::mafl_jakgui_1},
	{"mafl_jakgui_2", &CNpcScript::mafl_jakgui_2},
	{"mafl_jakgui_3", &CNpcScript::mafl_jakgui_3},
	{"mafl_jakgui_4", &CNpcScript::mafl_jakgui_4},
	{"mafl_jakgui_5", &CNpcScript::mafl_jakgui_5},
	{"mafl_jakgui_6", &CNpcScript::mafl_jakgui_6},
	{"mafl_jakgui_8", &CNpcScript::mafl_jakgui_8},
	{"mafl_jakgui_9", &CNpcScript::mafl_jakgui_9},
	{"mafl_jakgui_10", &CNpcScript::mafl_jakgui_10},
	{"mafl_ryar_0", &CNpcScript::mafl_ryar_0},
	{"mafl_ryar_1", &CNpcScript::mafl_ryar_1},
	{"mafl_ryar_2", &CNpcScript::mafl_ryar_2},
	{"mafl_ryar_3", &CNpcScript::mafl_ryar_3},
	{"mafl_ryar_4", &CNpcScript::mafl_ryar_4},
	{"mafl_ryar_5", &CNpcScript::mafl_ryar_5},
	{"mafl_ryar_6", &CNpcScript::mafl_ryar_6},
	{"mafl_ryar_8", &CNpcScript::mafl_ryar_8},
	{"mafl_ryar_9", &CNpcScript::mafl_ryar_9},
	{"mafl_ryar_10", &CNpcScript::mafl_ryar_10},
	{"mafl_donation_0", &CNpcScript::mafl_donation_0},
	{"mafl_donation_1", &CNpcScript::mafl_donation_1},
	{"mafl_donation_2", &CNpcScript::mafl_donation_2},
	{"mafl_donation_3", &CNpcScript::mafl_donation_3},
	{"mafl_donation_4", &CNpcScript::mafl_donation_4},
	{"mafl_donation_5", &CNpcScript::mafl_donation_5},
	{"mafl_donation_6", &CNpcScript::mafl_donation_6},
	{"mafl_donation_8", &CNpcScript::mafl_donation_8},
	{"mafl_donation_9", &CNpcScript::mafl_donation_9},
	{"mafl_donation_10", &CNpcScript::mafl_donation_10},
	{"mafl_goni_0", &CNpcScript::mafl_goni_0},
	{"mafl_goni_1", &CNpcScript::mafl_goni_1},
	{"mafl_goni_2", &CNpcScript::mafl_goni_2},
	{"mafl_goni_3", &CNpcScript::mafl_goni_3},
	{"mafl_goni_4", &CNpcScript::mafl_goni_4},
	{"mafl_goni_5", &CNpcScript::mafl_goni_5},
	{"mafl_goni_6", &CNpcScript::mafl_goni_6},
	{"mafl_goni_8", &CNpcScript::mafl_goni_8},
	{"mafl_goni_9", &CNpcScript::mafl_goni_9},
	{"mafl_goni_10", &CNpcScript::mafl_goni_10},
	{"mafl_festival_0", &CNpcScript::mafl_festival_0},
	{"mafl_festival_1", &CNpcScript::mafl_festival_1},
	{"mafl_festival_2", &CNpcScript::mafl_festival_2},
	{"mafl_festival_3", &CNpcScript::mafl_festival_3},
	{"mafl_festival_4", &CNpcScript::mafl_festival_4},
	{"mafl_festival_5", &CNpcScript::mafl_festival_5},
	{"mafl_festival_6", &CNpcScript::mafl_festival_6},
	{"mafl_festival_8", &CNpcScript::mafl_festival_8},
	{"mafl_festival_9", &CNpcScript::mafl_festival_9},
	{"mafl_festival_10", &CNpcScript::mafl_festival_10},
	{"mafl_costumenerupa_0", &CNpcScript::mafl_costumenerupa_0},
	{"mafl_costumenerupa_1", &CNpcScript::mafl_costumenerupa_1},
	{"mafl_costumenerupa_2", &CNpcScript::mafl_costumenerupa_2},
	{"mafl_costumenerupa_3", &CNpcScript::mafl_costumenerupa_3},
	{"mafl_costumenerupa_4", &CNpcScript::mafl_costumenerupa_4},
	{"mafl_costumenerupa_5", &CNpcScript::mafl_costumenerupa_5},
	{"mafl_costumenerupa_6", &CNpcScript::mafl_costumenerupa_6},
	{"mafl_costumenerupa_8", &CNpcScript::mafl_costumenerupa_8},
	{"mafl_costumenerupa_9", &CNpcScript::mafl_costumenerupa_9},
	{"mafl_costumenerupa_10", &CNpcScript::mafl_costumenerupa_10},
	{"mafl_flame_0", &CNpcScript::mafl_flame_0},
	{"mafl_flame_1", &CNpcScript::mafl_flame_1},
	{"mafl_flame_2", &CNpcScript::mafl_flame_2},
	{"mafl_flame_3", &CNpcScript::mafl_flame_3},
	{"mafl_flame_4", &CNpcScript::mafl_flame_4},
	{"mafl_flame_5", &CNpcScript::mafl_flame_5},
	{"mafl_flame_6", &CNpcScript::mafl_flame_6},
	{"mafl_flame_8", &CNpcScript::mafl_flame_8},
	{"mafl_flame_9", &CNpcScript::mafl_flame_9},
	{"mafl_flame_10", &CNpcScript::mafl_flame_10},
	{"masp_sanpres_0", &CNpcScript::masp_sanpres_0},
	{"masp_sanpres_1", &CNpcScript::masp_sanpres_1},
	{"masp_sanpres_2", &CNpcScript::masp_sanpres_2},
	{"masp_sanpres_3", &CNpcScript::masp_sanpres_3},
	{"masp_sanpres_4", &CNpcScript::masp_sanpres_4},
	{"masp_sanpres_5", &CNpcScript::masp_sanpres_5},
	{"masp_sanpres_6", &CNpcScript::masp_sanpres_6},
	{"masp_sanpres_8", &CNpcScript::masp_sanpres_8},
	{"masp_sanpres_9", &CNpcScript::masp_sanpres_9},
	{"masp_sanpres_10", &CNpcScript::masp_sanpres_10},
	{"mahe_raelra_0", &CNpcScript::mahe_raelra_0},
	{"mahe_raelra_1", &CNpcScript::mahe_raelra_1},
	{"mahe_raelra_2", &CNpcScript::mahe_raelra_2},
	{"mahe_raelra_3", &CNpcScript::mahe_raelra_3},
	{"mahe_raelra_4", &CNpcScript::mahe_raelra_4},
	{"mahe_raelra_5", &CNpcScript::mahe_raelra_5},
	{"mahe_raelra_6", &CNpcScript::mahe_raelra_6},
	{"mahe_raelra_8", &CNpcScript::mahe_raelra_8},
	{"mahe_raelra_9", &CNpcScript::mahe_raelra_9},
	{"mahe_raelra_10", &CNpcScript::mahe_raelra_10},
	{"masp_sancherco_0", &CNpcScript::masp_sancherco_0},
	{"masp_sancherco_1", &CNpcScript::masp_sancherco_1},
	{"masp_sancherco_2", &CNpcScript::masp_sancherco_2},
	{"masp_sancherco_3", &CNpcScript::masp_sancherco_3},
	{"masp_sancherco_4", &CNpcScript::masp_sancherco_4},
	{"masp_sancherco_5", &CNpcScript::masp_sancherco_5},
	{"masp_sancherco_6", &CNpcScript::masp_sancherco_6},
	{"masp_sancherco_8", &CNpcScript::masp_sancherco_8},
	{"masp_sancherco_9", &CNpcScript::masp_sancherco_9},
	{"masp_sancherco_10", &CNpcScript::masp_sancherco_10},
	{"mahe_raila_0", &CNpcScript::mahe_raila_0},
	{"mahe_raila_1", &CNpcScript::mahe_raila_1},
	{"mahe_raila_2", &CNpcScript::mahe_raila_2},
	{"mahe_raila_3", &CNpcScript::mahe_raila_3},
	{"mahe_raila_4", &CNpcScript::mahe_raila_4},
	{"mahe_raila_5", &CNpcScript::mahe_raila_5},
	{"mahe_raila_6", &CNpcScript::mahe_raila_6},
	{"mahe_raila_8", &CNpcScript::mahe_raila_8},
	{"mahe_raila_9", &CNpcScript::mahe_raila_9},
	{"mahe_raila_10", &CNpcScript::mahe_raila_10},
	{"masp_tomas_0", &CNpcScript::masp_tomas_0},
	{"masp_tomas_1", &CNpcScript::masp_tomas_1},
	{"masp_tomas_2", &CNpcScript::masp_tomas_2},
	{"masp_tomas_3", &CNpcScript::masp_tomas_3},
	{"masp_tomas_4", &CNpcScript::masp_tomas_4},
	{"masp_tomas_5", &CNpcScript::masp_tomas_5},
	{"masp_tomas_6", &CNpcScript::masp_tomas_6},
	{"masp_tomas_8", &CNpcScript::masp_tomas_8},
	{"masp_tomas_9", &CNpcScript::masp_tomas_9},
	{"masp_tomas_10", &CNpcScript::masp_tomas_10},
	{"mahe_reo_0", &CNpcScript::mahe_reo_0},
	{"mahe_reo_1", &CNpcScript::mahe_reo_1},
	{"mahe_reo_2", &CNpcScript::mahe_reo_2},
	{"mahe_reo_3", &CNpcScript::mahe_reo_3},
	{"mahe_reo_4", &CNpcScript::mahe_reo_4},
	{"mahe_reo_5", &CNpcScript::mahe_reo_5},
	{"mahe_reo_6", &CNpcScript::mahe_reo_6},
	{"mahe_reo_8", &CNpcScript::mahe_reo_8},
	{"mahe_reo_9", &CNpcScript::mahe_reo_9},
	{"mahe_reo_10", &CNpcScript::mahe_reo_10},
	{"masp_smith_0", &CNpcScript::masp_smith_0},
	{"masp_smith_1", &CNpcScript::masp_smith_1},
	{"masp_smith_2", &CNpcScript::masp_smith_2},
	{"masp_smith_3", &CNpcScript::masp_smith_3},
	{"masp_smith_4", &CNpcScript::masp_smith_4},
	{"masp_smith_5", &CNpcScript::masp_smith_5},
	{"masp_smith_6", &CNpcScript::masp_smith_6},
	{"masp_smith_8", &CNpcScript::masp_smith_8},
	{"masp_smith_9", &CNpcScript::masp_smith_9},
	{"masp_smith_10", &CNpcScript::masp_smith_10},
	{"mahe_rio_0", &CNpcScript::mahe_rio_0},
	{"mahe_rio_1", &CNpcScript::mahe_rio_1},
	{"mahe_rio_2", &CNpcScript::mahe_rio_2},
	{"mahe_rio_3", &CNpcScript::mahe_rio_3},
	{"mahe_rio_4", &CNpcScript::mahe_rio_4},
	{"mahe_rio_5", &CNpcScript::mahe_rio_5},
	{"mahe_rio_6", &CNpcScript::mahe_rio_6},
	{"mahe_rio_8", &CNpcScript::mahe_rio_8},
	{"mahe_rio_9", &CNpcScript::mahe_rio_9},
	{"mahe_rio_10", &CNpcScript::mahe_rio_10},
	{"mafl_reonan_0", &CNpcScript::mafl_reonan_0},
	{"mafl_reonan_1", &CNpcScript::mafl_reonan_1},
	{"mafl_reonan_2", &CNpcScript::mafl_reonan_2},
	{"mafl_reonan_3", &CNpcScript::mafl_reonan_3},
	{"mafl_reonan_4", &CNpcScript::mafl_reonan_4},
	{"mafl_reonan_5", &CNpcScript::mafl_reonan_5},
	{"mafl_reonan_6", &CNpcScript::mafl_reonan_6},
	{"mafl_reonan_8", &CNpcScript::mafl_reonan_8},
	{"mafl_reonan_9", &CNpcScript::mafl_reonan_9},
	{"mafl_reonan_10", &CNpcScript::mafl_reonan_10},
	{"mafl_nerco_0", &CNpcScript::mafl_nerco_0},
	{"mafl_nerco_1", &CNpcScript::mafl_nerco_1},
	{"mafl_nerco_2", &CNpcScript::mafl_nerco_2},
	{"mafl_nerco_3", &CNpcScript::mafl_nerco_3},
	{"mafl_nerco_4", &CNpcScript::mafl_nerco_4},
	{"mafl_nerco_5", &CNpcScript::mafl_nerco_5},
	{"mafl_nerco_6", &CNpcScript::mafl_nerco_6},
	{"mafl_nerco_8", &CNpcScript::mafl_nerco_8},
	{"mafl_nerco_9", &CNpcScript::mafl_nerco_9},
	{"mafl_nerco_10", &CNpcScript::mafl_nerco_10},
	{"mafl_hael_0", &CNpcScript::mafl_hael_0},
	{"mafl_hael_1", &CNpcScript::mafl_hael_1},
	{"mafl_hael_2", &CNpcScript::mafl_hael_2},
	{"mafl_hael_3", &CNpcScript::mafl_hael_3},
	{"mafl_hael_4", &CNpcScript::mafl_hael_4},
	{"mafl_hael_5", &CNpcScript::mafl_hael_5},
	{"mafl_hael_6", &CNpcScript::mafl_hael_6},
	{"mafl_hael_8", &CNpcScript::mafl_hael_8},
	{"mafl_hael_9", &CNpcScript::mafl_hael_9},
	{"mafl_hael_10", &CNpcScript::mafl_hael_10},
	{"masa_luzina_0", &CNpcScript::masa_luzina_0},
	{"masa_luzina_1", &CNpcScript::masa_luzina_1},
	{"masa_luzina_2", &CNpcScript::masa_luzina_2},
	{"masa_luzina_3", &CNpcScript::masa_luzina_3},
	{"masa_luzina_4", &CNpcScript::masa_luzina_4},
	{"masa_luzina_5", &CNpcScript::masa_luzina_5},
	{"masa_luzina_6", &CNpcScript::masa_luzina_6},
	{"masa_luzina_8", &CNpcScript::masa_luzina_8},
	{"masa_luzina_9", &CNpcScript::masa_luzina_9},
	{"masa_luzina_10", &CNpcScript::masa_luzina_10},
	{"masa_nercha_0", &CNpcScript::masa_nercha_0},
	{"masa_nercha_1", &CNpcScript::masa_nercha_1},
	{"masa_nercha_2", &CNpcScript::masa_nercha_2},
	{"masa_nercha_3", &CNpcScript::masa_nercha_3},
	{"masa_nercha_4", &CNpcScript::masa_nercha_4},
	{"masa_nercha_5", &CNpcScript::masa_nercha_5},
	{"masa_nercha_6", &CNpcScript::masa_nercha_6},
	{"masa_nercha_8", &CNpcScript::masa_nercha_8},
	{"masa_nercha_9", &CNpcScript::masa_nercha_9},
	{"masa_nercha_10", &CNpcScript::masa_nercha_10},
	{"mafl_reodos_0", &CNpcScript::mafl_reodos_0},
	{"mafl_reodos_1", &CNpcScript::mafl_reodos_1},
	{"mafl_reodos_2", &CNpcScript::mafl_reodos_2},
	{"mafl_reodos_3", &CNpcScript::mafl_reodos_3},
	{"mafl_reodos_4", &CNpcScript::mafl_reodos_4},
	{"mafl_reodos_5", &CNpcScript::mafl_reodos_5},
	{"mafl_reodos_6", &CNpcScript::mafl_reodos_6},
	{"mafl_reodos_8", &CNpcScript::mafl_reodos_8},
	{"mafl_reodos_9", &CNpcScript::mafl_reodos_9},
	{"mafl_reodos_10", &CNpcScript::mafl_reodos_10},
	{"mafl_hendel_0", &CNpcScript::mafl_hendel_0},
	{"mafl_hendel_1", &CNpcScript::mafl_hendel_1},
	{"mafl_hendel_2", &CNpcScript::mafl_hendel_2},
	{"mafl_hendel_3", &CNpcScript::mafl_hendel_3},
	{"mafl_hendel_4", &CNpcScript::mafl_hendel_4},
	{"mafl_hendel_5", &CNpcScript::mafl_hendel_5},
	{"mafl_hendel_6", &CNpcScript::mafl_hendel_6},
	{"mafl_hendel_8", &CNpcScript::mafl_hendel_8},
	{"mafl_hendel_9", &CNpcScript::mafl_hendel_9},
	{"mafl_hendel_10", &CNpcScript::mafl_hendel_10},
	{"mafl_zoro_0", &CNpcScript::mafl_zoro_0},
	{"mafl_zoro_1", &CNpcScript::mafl_zoro_1},
	{"mafl_zoro_2", &CNpcScript::mafl_zoro_2},
	{"mafl_zoro_3", &CNpcScript::mafl_zoro_3},
	{"mafl_zoro_4", &CNpcScript::mafl_zoro_4},
	{"mafl_zoro_5", &CNpcScript::mafl_zoro_5},
	{"mafl_zoro_6", &CNpcScript::mafl_zoro_6},
	{"mafl_zoro_8", &CNpcScript::mafl_zoro_8},
	{"mafl_zoro_9", &CNpcScript::mafl_zoro_9},
	{"mafl_zoro_10", &CNpcScript::mafl_zoro_10},
	{"mafl_ellain_0", &CNpcScript::mafl_ellain_0},
	{"mafl_ellain_1", &CNpcScript::mafl_ellain_1},
	{"mafl_ellain_2", &CNpcScript::mafl_ellain_2},
	{"mafl_ellain_3", &CNpcScript::mafl_ellain_3},
	{"mafl_ellain_4", &CNpcScript::mafl_ellain_4},
	{"mafl_ellain_5", &CNpcScript::mafl_ellain_5},
	{"mafl_ellain_6", &CNpcScript::mafl_ellain_6},
	{"mafl_ellain_8", &CNpcScript::mafl_ellain_8},
	{"mafl_ellain_9", &CNpcScript::mafl_ellain_9},
	{"mafl_ellain_10", &CNpcScript::mafl_ellain_10},
	{"mafl_ainher_0", &CNpcScript::mafl_ainher_0},
	{"mafl_ainher_1", &CNpcScript::mafl_ainher_1},
	{"mafl_ainher_2", &CNpcScript::mafl_ainher_2},
	{"mafl_ainher_3", &CNpcScript::mafl_ainher_3},
	{"mafl_ainher_4", &CNpcScript::mafl_ainher_4},
	{"mafl_ainher_5", &CNpcScript::mafl_ainher_5},
	{"mafl_ainher_6", &CNpcScript::mafl_ainher_6},
	{"mafl_ainher_8", &CNpcScript::mafl_ainher_8},
	{"mafl_ainher_9", &CNpcScript::mafl_ainher_9},
	{"mafl_ainher_10", &CNpcScript::mafl_ainher_10},
	{"mafl_sury_0", &CNpcScript::mafl_sury_0},
	{"mafl_sury_1", &CNpcScript::mafl_sury_1},
	{"mafl_sury_2", &CNpcScript::mafl_sury_2},
	{"mafl_sury_3", &CNpcScript::mafl_sury_3},
	{"mafl_sury_4", &CNpcScript::mafl_sury_4},
	{"mafl_sury_5", &CNpcScript::mafl_sury_5},
	{"mafl_sury_6", &CNpcScript::mafl_sury_6},
	{"mafl_sury_8", &CNpcScript::mafl_sury_8},
	{"mafl_sury_9", &CNpcScript::mafl_sury_9},
	{"mafl_sury_10", &CNpcScript::mafl_sury_10},
	{"dudk_drico_0", &CNpcScript::dudk_drico_0},
	{"dudk_drico_1", &CNpcScript::dudk_drico_1},
	{"dudk_drico_2", &CNpcScript::dudk_drico_2},
	{"dudk_drico_3", &CNpcScript::dudk_drico_3},
	{"dudk_drico_4", &CNpcScript::dudk_drico_4},
	{"dudk_drico_5", &CNpcScript::dudk_drico_5},
	{"dudk_drico_6", &CNpcScript::dudk_drico_6},
	{"dudk_drico_8", &CNpcScript::dudk_drico_8},
	{"dudk_drico_9", &CNpcScript::dudk_drico_9},
	{"dudk_drico_10", &CNpcScript::dudk_drico_10},

	{"mafl_fwcenter_0", &CNpcScript::mafl_fwcenter_0},
	{"mafl_fwcenter_1", &CNpcScript::mafl_fwcenter_1},
	{"mafl_fwcenter_2", &CNpcScript::mafl_fwcenter_2},
	{"mafl_fwcenter_3", &CNpcScript::mafl_fwcenter_3},
	{"mafl_fwcenter_4", &CNpcScript::mafl_fwcenter_4},
	{"mafl_fwcenter_5", &CNpcScript::mafl_fwcenter_5},
	{"mafl_fwcenter_6", &CNpcScript::mafl_fwcenter_6},
	{"mafl_fwcenter_8", &CNpcScript::mafl_fwcenter_8},
	{"mafl_fwcenter_9", &CNpcScript::mafl_fwcenter_9},
	{"mafl_fwcenter_10", &CNpcScript::mafl_fwcenter_10},

	{"dudk_shain_0", &CNpcScript::mafl_shain_0},
	{"mafl_shain_1", &CNpcScript::mafl_shain_1},
	{"mafl_shain_2", &CNpcScript::mafl_shain_2},
	{"mafl_shain_3", &CNpcScript::mafl_shain_3},
	{"mafl_shain_4", &CNpcScript::mafl_shain_4},
	{"mafl_shain_5", &CNpcScript::mafl_shain_5},
	{"mafl_shain_6", &CNpcScript::mafl_shain_6},
	{"mafl_shain_8", &CNpcScript::mafl_shain_8},
	{"mafl_shain_9", &CNpcScript::mafl_shain_9},
	{"mafl_shain_10", &CNpcScript::mafl_shain_10},
	{"mafl_rangpang_0", &CNpcScript::mafl_rangpang_0},
	{"mafl_rangpang_1", &CNpcScript::mafl_rangpang_1},
	{"mafl_rangpang_2", &CNpcScript::mafl_rangpang_2},
	{"mafl_rangpang_3", &CNpcScript::mafl_rangpang_3},
	{"mafl_rangpang_4", &CNpcScript::mafl_rangpang_4},
	{"mafl_rangpang_5", &CNpcScript::mafl_rangpang_5},
	{"mafl_rangpang_6", &CNpcScript::mafl_rangpang_6},
	{"mafl_rangpang_8", &CNpcScript::mafl_rangpang_8},
	{"mafl_rangpang_9", &CNpcScript::mafl_rangpang_9},
	{"mafl_rangpang_10", &CNpcScript::mafl_rangpang_10},
	{"mafl_poneang_0", &CNpcScript::mafl_poneang_0},
	{"mafl_poneang_1", &CNpcScript::mafl_poneang_1},
	{"mafl_poneang_2", &CNpcScript::mafl_poneang_2},
	{"mafl_poneang_3", &CNpcScript::mafl_poneang_3},
	{"mafl_poneang_4", &CNpcScript::mafl_poneang_4},
	{"mafl_poneang_5", &CNpcScript::mafl_poneang_5},
	{"mafl_poneang_6", &CNpcScript::mafl_poneang_6},
	{"mafl_poneang_8", &CNpcScript::mafl_poneang_8},
	{"mafl_poneang_9", &CNpcScript::mafl_poneang_9},
	{"mafl_poneang_10", &CNpcScript::mafl_poneang_10},
	{"mafl_reong_0", &CNpcScript::mafl_reong_0},
	{"mafl_reong_1", &CNpcScript::mafl_reong_1},
	{"mafl_reong_2", &CNpcScript::mafl_reong_2},
	{"mafl_reong_3", &CNpcScript::mafl_reong_3},
	{"mafl_reong_4", &CNpcScript::mafl_reong_4},
	{"mafl_reong_5", &CNpcScript::mafl_reong_5},
	{"mafl_reong_6", &CNpcScript::mafl_reong_6},
	{"mafl_reong_8", &CNpcScript::mafl_reong_8},
	{"mafl_reong_9", &CNpcScript::mafl_reong_9},
	{"mafl_reong_10", &CNpcScript::mafl_reong_10},
	{"mafl_tamtam_0", &CNpcScript::mafl_tamtam_0},
	{"mafl_tamtam_1", &CNpcScript::mafl_tamtam_1},
	{"mafl_tamtam_2", &CNpcScript::mafl_tamtam_2},
	{"mafl_tamtam_3", &CNpcScript::mafl_tamtam_3},
	{"mafl_tamtam_4", &CNpcScript::mafl_tamtam_4},
	{"mafl_tamtam_5", &CNpcScript::mafl_tamtam_5},
	{"mafl_tamtam_6", &CNpcScript::mafl_tamtam_6},
	{"mafl_tamtam_8", &CNpcScript::mafl_tamtam_8},
	{"mafl_tamtam_9", &CNpcScript::mafl_tamtam_9},
	{"mafl_tamtam_10", &CNpcScript::mafl_tamtam_10},
	{"mafl_ranggo_0", &CNpcScript::mafl_ranggo_0},
	{"mafl_ranggo_1", &CNpcScript::mafl_ranggo_1},
	{"mafl_ranggo_2", &CNpcScript::mafl_ranggo_2},
	{"mafl_ranggo_3", &CNpcScript::mafl_ranggo_3},
	{"mafl_ranggo_4", &CNpcScript::mafl_ranggo_4},
	{"mafl_ranggo_5", &CNpcScript::mafl_ranggo_5},
	{"mafl_ranggo_6", &CNpcScript::mafl_ranggo_6},
	{"mafl_ranggo_8", &CNpcScript::mafl_ranggo_8},
	{"mafl_ranggo_9", &CNpcScript::mafl_ranggo_9},
	{"mafl_ranggo_10", &CNpcScript::mafl_ranggo_10},
	{"mafl_toto_0", &CNpcScript::mafl_toto_0},
	{"mafl_toto_1", &CNpcScript::mafl_toto_1},
	{"mafl_toto_2", &CNpcScript::mafl_toto_2},
	{"mafl_toto_3", &CNpcScript::mafl_toto_3},
	{"mafl_toto_4", &CNpcScript::mafl_toto_4},
	{"mafl_toto_5", &CNpcScript::mafl_toto_5},
	{"mafl_toto_6", &CNpcScript::mafl_toto_6},
	{"mafl_toto_8", &CNpcScript::mafl_toto_8},
	{"mafl_toto_9", &CNpcScript::mafl_toto_9},
	{"mafl_toto_10", &CNpcScript::mafl_toto_10},
	{"mafl_sebrance_0", &CNpcScript::mafl_sebrance_0},
	{"mafl_sebrance_1", &CNpcScript::mafl_sebrance_1},
	{"mafl_sebrance_2", &CNpcScript::mafl_sebrance_2},
	{"mafl_sebrance_3", &CNpcScript::mafl_sebrance_3},
	{"mafl_sebrance_4", &CNpcScript::mafl_sebrance_4},
	{"mafl_sebrance_5", &CNpcScript::mafl_sebrance_5},
	{"mafl_sebrance_6", &CNpcScript::mafl_sebrance_6},
	{"mafl_sebrance_8", &CNpcScript::mafl_sebrance_8},
	{"mafl_sebrance_9", &CNpcScript::mafl_sebrance_9},
	{"mafl_sebrance_10", &CNpcScript::mafl_sebrance_10},
	{"mafl_donjobanni_0", &CNpcScript::mafl_donjobanni_0},
	{"mafl_donjobanni_1", &CNpcScript::mafl_donjobanni_1},
	{"mafl_donjobanni_2", &CNpcScript::mafl_donjobanni_2},
	{"mafl_donjobanni_3", &CNpcScript::mafl_donjobanni_3},
	{"mafl_donjobanni_4", &CNpcScript::mafl_donjobanni_4},
	{"mafl_donjobanni_5", &CNpcScript::mafl_donjobanni_5},
	{"mafl_donjobanni_6", &CNpcScript::mafl_donjobanni_6},
	{"mafl_donjobanni_8", &CNpcScript::mafl_donjobanni_8},
	{"mafl_donjobanni_9", &CNpcScript::mafl_donjobanni_9},
	{"mafl_donjobanni_10", &CNpcScript::mafl_donjobanni_10},
	{"mafl_mordolrin_0", &CNpcScript::mafl_mordolrin_0},
	{"mafl_mordolrin_1", &CNpcScript::mafl_mordolrin_1},
	{"mafl_mordolrin_2", &CNpcScript::mafl_mordolrin_2},
	{"mafl_mordolrin_3", &CNpcScript::mafl_mordolrin_3},
	{"mafl_mordolrin_4", &CNpcScript::mafl_mordolrin_4},
	{"mafl_mordolrin_5", &CNpcScript::mafl_mordolrin_5},
	{"mafl_mordolrin_6", &CNpcScript::mafl_mordolrin_6},
	{"mafl_mordolrin_8", &CNpcScript::mafl_mordolrin_8},
	{"mafl_mordolrin_9", &CNpcScript::mafl_mordolrin_9},
	{"mafl_mordolrin_10", &CNpcScript::mafl_mordolrin_10},
	{"mafl_jarcoba_0", &CNpcScript::mafl_jarcoba_0},
	{"mafl_jarcoba_1", &CNpcScript::mafl_jarcoba_1},
	{"mafl_jarcoba_2", &CNpcScript::mafl_jarcoba_2},
	{"mafl_jarcoba_3", &CNpcScript::mafl_jarcoba_3},
	{"mafl_jarcoba_4", &CNpcScript::mafl_jarcoba_4},
	{"mafl_jarcoba_5", &CNpcScript::mafl_jarcoba_5},
	{"mafl_jarcoba_6", &CNpcScript::mafl_jarcoba_6},
	{"mafl_jarcoba_8", &CNpcScript::mafl_jarcoba_8},
	{"mafl_jarcoba_9", &CNpcScript::mafl_jarcoba_9},
	{"mafl_jarcoba_10", &CNpcScript::mafl_jarcoba_10},
	{"mafl_moneyjoba_0", &CNpcScript::mafl_moneyjoba_0},
	{"mafl_moneyjoba_1", &CNpcScript::mafl_moneyjoba_1},
	{"mafl_moneyjoba_2", &CNpcScript::mafl_moneyjoba_2},
	{"mafl_moneyjoba_3", &CNpcScript::mafl_moneyjoba_3},
	{"mafl_moneyjoba_4", &CNpcScript::mafl_moneyjoba_4},
	{"mafl_moneyjoba_5", &CNpcScript::mafl_moneyjoba_5},
	{"mafl_moneyjoba_6", &CNpcScript::mafl_moneyjoba_6},
	{"mafl_moneyjoba_8", &CNpcScript::mafl_moneyjoba_8},
	{"mafl_moneyjoba_9", &CNpcScript::mafl_moneyjoba_9},
	{"mafl_moneyjoba_10", &CNpcScript::mafl_moneyjoba_10},
	{"mafl_givemoney_0", &CNpcScript::mafl_givemoney_0},
	{"mafl_givemoney_1", &CNpcScript::mafl_givemoney_1},
	{"mafl_givemoney_2", &CNpcScript::mafl_givemoney_2},
	{"mafl_givemoney_3", &CNpcScript::mafl_givemoney_3},
	{"mafl_givemoney_4", &CNpcScript::mafl_givemoney_4},
	{"mafl_givemoney_5", &CNpcScript::mafl_givemoney_5},
	{"mafl_givemoney_6", &CNpcScript::mafl_givemoney_6},
	{"mafl_givemoney_8", &CNpcScript::mafl_givemoney_8},
	{"mafl_givemoney_9", &CNpcScript::mafl_givemoney_9},
	{"mafl_givemoney_10", &CNpcScript::mafl_givemoney_10},
	{"mafl_maple_0", &CNpcScript::mafl_maple_0},
	{"mafl_maple_1", &CNpcScript::mafl_maple_1},
	{"mafl_maple_2", &CNpcScript::mafl_maple_2},
	{"mafl_maple_3", &CNpcScript::mafl_maple_3},
	{"mafl_maple_4", &CNpcScript::mafl_maple_4},
	{"mafl_maple_5", &CNpcScript::mafl_maple_5},
	{"mafl_maple_6", &CNpcScript::mafl_maple_6},
	{"mafl_maple_8", &CNpcScript::mafl_maple_8},
	{"mafl_maple_9", &CNpcScript::mafl_maple_9},
	{"mafl_maple_10", &CNpcScript::mafl_maple_10},
	{"mafl_tifa_0", &CNpcScript::mafl_tifa_0},
	{"mafl_tifa_1", &CNpcScript::mafl_tifa_1},
	{"mafl_tifa_2", &CNpcScript::mafl_tifa_2},
	{"mafl_tifa_3", &CNpcScript::mafl_tifa_3},
	{"mafl_tifa_4", &CNpcScript::mafl_tifa_4},
	{"mafl_tifa_5", &CNpcScript::mafl_tifa_5},
	{"mafl_tifa_6", &CNpcScript::mafl_tifa_6},
	{"mafl_tifa_8", &CNpcScript::mafl_tifa_8},
	{"mafl_tifa_9", &CNpcScript::mafl_tifa_9},
	{"mafl_tifa_10", &CNpcScript::mafl_tifa_10},
	{"mafl_snakedak_0", &CNpcScript::mafl_snakedak_0},
	{"mafl_snakedak_1", &CNpcScript::mafl_snakedak_1},
	{"mafl_snakedak_2", &CNpcScript::mafl_snakedak_2},
	{"mafl_snakedak_3", &CNpcScript::mafl_snakedak_3},
	{"mafl_snakedak_4", &CNpcScript::mafl_snakedak_4},
	{"mafl_snakedak_5", &CNpcScript::mafl_snakedak_5},
	{"mafl_snakedak_6", &CNpcScript::mafl_snakedak_6},
	{"mafl_snakedak_8", &CNpcScript::mafl_snakedak_8},
	{"mafl_snakedak_9", &CNpcScript::mafl_snakedak_9},
	{"mafl_snakedak_10", &CNpcScript::mafl_snakedak_10},


};

static map< string, PFN > g_functions;

void InitFunctions()
{
	string str;
	
	int n = sizeof( g_tables ) / sizeof( g_tables[0] );
	for(int i=0; i<n; ++i)
	{
		str = g_tables[i].name;
		g_functions.insert( make_pair( str, g_tables[i].pmf ) );
	}
}


PFN GetFunction( const char* szFunction )
{
	map< string, PFN >::iterator it = g_functions.find( szFunction );
	if( it != g_functions.end() )
		return  it->second;
	else
		return NULL;
}



////////////////////////////////////////////////////////////////////////////////
//
//

// TODO
extern LPCTSTR GetDialogText( DWORD dwText );	// NpcScript.cpp 

//int CNpcScript::GetVal()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetVal( m_pInfo );
//		}
//	}
//
//	return 0;
//}

int CNpcScript::GetSrcId()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetSrcId( m_pInfo );
		}
	}

	return 0;
}

int CNpcScript::GetDstId()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetDstId( m_pInfo );
		}
	}

	return 0;
}

int CNpcScript::NpcId()
{
	return GetDstId();
}


void CNpcScript::Say( DWORD szMsg )
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			LPCTSTR lpszMsg = GetDialogText( szMsg );
			if( lpszMsg == NULL )
			{
				return;
			}

			pTable->Say( m_pInfo, lpszMsg );
		}
	}
}


void CNpcScript::Speak(int nId,DWORD szStr)
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			LPCTSTR lpszMsg = GetDialogText( szStr );
			if( lpszMsg == NULL )
			{
				return;
			}

			pTable->Speak( m_pInfo, nId, lpszMsg );
		}
	}
}

int CNpcScript::Random( int n )
{
	if( n > 0 )
	{
		return rand() % n;
	}
	
	return 0;
}

void CNpcScript::Replace( int nWorld, float x, float y, float z )
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->Replace( m_pInfo, nWorld, x, y, z );
		}
	}
}


int CNpcScript::GetPlayerLvl()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetPlayerLvl( m_pInfo );
		}
	}

	return 0;
}

int CNpcScript::GetPlayerJob()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetPlayerJob( m_pInfo );
		}
	}

	return 0;
}


int CNpcScript::GetQuestState(int nQuest)
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetQuestState( m_pInfo, nQuest );
		}
	}

	return 0;
}

int CNpcScript::IsSetQuest(int nQuest)
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsSetQuest( m_pInfo, nQuest );
		}
	}

	return 0;
}

void CNpcScript::CreateItem( DWORD dwID, int nCount)  
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->CreateItem( m_pInfo, dwID, nCount );
		}
	}
}

void CNpcScript::RemoveGold( int nGold )   
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->RemoveGold( m_pInfo, nGold );
		}
	}
}

int CNpcScript::GetPlayerGold()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetPlayerGold( m_pInfo );
		}
	}

	return 0;
}


int CNpcScript::IsGuildQuest( int nQuest )	
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsGuildQuest( m_pInfo, nQuest );
		}
	}

	return 0;
}

int CNpcScript::GetGuildQuestState( int nQuest )
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetGuildQuestState( m_pInfo, nQuest );
		}
	}

	return 0;
}

// if( IsWormonServer() == TRUE )
int CNpcScript::IsWormonServer() 
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsWormonServer( m_pInfo );
		}
	}

	return 0;
}

// if( MonHuntStart( QUEST_WARMON_LV1, QS_BEGIN, QS_END, 1 ) == FALSE )
int CNpcScript::MonHuntStart(int nQuest, int nState, int nState2, int n )   
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->MonHuntStart( m_pInfo, nQuest, nState, nState2, n );
		}
	}

	return 0;
}

// if( MonHuntStartParty( QUEST_BOSS_LV1, QS_BEGIN, QS_END, 1 ) == TRUE )
int CNpcScript::MonHuntStartParty(int nQuest, int nState, int nState2, int n ) 
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->MonHuntStartParty( m_pInfo, nQuest, nState, nState2, n );
		}
	}

	return 0;
}

// DropQuestItem( II_SYS_SYS_QUE_HEROTALE01, 1000 );	,	
//void CNpcScript::DropQuestItem( DWORD dwID, DWORD dwProb )  
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->DropQuestItem( m_pInfo, dwID, dwProb );
//		}
//	}
//}

// ExpQuestEffect(NPCDIALOG_INFO* pInfo, DWORD dwID, BOOL bNPC )
//void CNpcScript::ExpQuestEffect( DWORD dwID, BOOL bNPC ) 	
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->ExpQuestEffect( m_pInfo, dwID, bNPC );
//		}
//	}
//}

// RemoveItem( II_SYS_SYS_QUE_SBURUDENGTOOTH, 10 );	
void CNpcScript::RemoveItem(DWORD dwID, int nCount )	
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->RemoveItem( m_pInfo, dwID, nCount );
		}
	}
}

// if( GetLocalEventState(0) == 1 ) 
//int CNpcScript::GetLocalEventState( int nEvent ) 
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetLocalEventState( m_pInfo, nEvent );
//		}
//	}
//
//	return 0;
//}

void CNpcScript::InitStat()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->InitStat( m_pInfo );
		}
	}
}

//void CNpcScript::InitStr()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->InitStr( m_pInfo );
//		}
//	}
//}

//void CNpcScript::InitSta()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->InitSta( m_pInfo );
//		}
//	}
//}

//void CNpcScript::InitDex()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->InitDex( m_pInfo );
//		}
//	}
//}
//
//void CNpcScript::InitInt()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->InitInt( m_pInfo );
//		}
//	}
//}
 
// GetItemNum(II_SYS_SYS_QUE_BOMBTIMER) >= 5
int  CNpcScript::GetItemNum( DWORD dwID )        
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetItemNum( m_pInfo, dwID );
		}
	}

	return 0;
}

// RemoveAllItem( II_SYS_SYS_QUE_HEROLIFE01 );
void CNpcScript::RemoveAllItem( DWORD dwID )     
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->RemoveAllItem( m_pInfo, dwID );
		}
	}
}
// ChangeJob( 10 );
void CNpcScript::ChangeJob(int nJob )  	
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->ChangeJob( m_pInfo, nJob );
		}
	}
}
// if( GetPlayerSex() == 0)
//int CNpcScript::GetPlayerSex()      
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetPlayerSex( m_pInfo );
//		}
//	}
//
//	return 0;
//}
//EquipItem( II_ARM_S_CLO_CLO_SYSCLOAK03 );
//void CNpcScript::EquipItem( DWORD dwID )  
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->EquipItem( m_pInfo, dwID );
//		}
//	}
//}

//int CNpcScript::IsPlayerSkillPoint() 
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->IsPlayerSkillPoint( m_pInfo );
//		}
//	}
//
//	return 0;
//}

// /SetScriptTimer( 15 );
//void CNpcScript::SetScriptTimer(int /*nTimer*/)   
//{
//// do nothing
//}


void CNpcScript::AddKey( DWORD szWord )           
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			LPCTSTR lpszWord = GetDialogText( szWord );
			if( lpszWord == NULL )
			{
				return;
			}

			pTable->AddKey( m_pInfo, lpszWord, "", 0 );
		}
	}
}

//void CNpcScript::AddKey( DWORD szWord, DWORD szKey )           
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			LPCTSTR lpszWord = GetDialogText( szWord );
//			LPCTSTR lpszKey = GetDialogText( szKey );
//			if( lpszWord == NULL || lpszKey == NULL )
//			{
//				return;
//			}
//
//			pTable->AddKey( m_pInfo, lpszWord, lpszKey, 0 );
//		}
//	}
//}
//
//void CNpcScript::AddKey( DWORD szWord , DWORD szKey, int nParam )          
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			LPCTSTR lpszWord = GetDialogText( szWord );
//			LPCTSTR lpszKey = GetDialogText( szKey );
//			if( lpszWord == NULL || lpszKey == NULL )
//			{
//				return;
//			}
//
//			pTable->AddKey( m_pInfo, lpszWord, lpszKey, nParam );
//		}
//	}
//}

//void CNpcScript::AddAnswer( DWORD szWord, DWORD szKey, int n )          
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			LPCTSTR lpszWord = GetDialogText( szWord );
//			LPCTSTR lpszKey = GetDialogText( szKey );
//			if( lpszWord == NULL || lpszKey == NULL )
//			{
//				return;
//			}
//
//			pTable->AddAnswer( m_pInfo, lpszWord, lpszKey, n );
//		}
//	}
//}
//
//void CNpcScript::AddAnswer( DWORD szWord )  
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			LPCTSTR lpszWord = GetDialogText( szWord );
//			if( lpszWord == NULL )
//			{
//				return;
//			}
//
//			pTable->AddAnswer( m_pInfo, lpszWord, "", 0 );
//		}
//	}
//}

void CNpcScript::RemoveKey(DWORD szKey )  // RemoveKey( "데카네스" );
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			LPCTSTR lpszKey = GetDialogText( szKey );
			if( lpszKey == NULL )
			{
				return;
			}

			pTable->RemoveKey( m_pInfo, lpszKey );
		}
	}
}
//
//void CNpcScript::RemoveAllKey()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->RemoveAllKey( m_pInfo );
//		}
//	}
//}

//void CNpcScript::SetMark()
//{
//	// do nothing
//}
//
//void CNpcScript::GoMark()
//{
//	// do nothing
//}

int CNpcScript::IsParty()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsParty( m_pInfo );
		}
	}

	return 0;
}

int CNpcScript::IsPartyMaster()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsPartyMaster( m_pInfo );
		}
	}

	return 0;
}

int CNpcScript::GetPartyNum()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetPartyNum( m_pInfo );
		}
	}

	return 0;
}

//int CNpcScript::GetPartyLevel()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetPartyLevel( m_pInfo );
//		}
//	}
//
//	return 0;
//}

int CNpcScript::IsPartyGuild() // IsPartyGuild() == 2
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsPartyGuild( m_pInfo );
		}
	}

	return 0;
}

int CNpcScript::IsGuild()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsGuild( m_pInfo );
		}
	}

	return 0;
}

int CNpcScript::IsGuildMaster()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->IsGuildMaster( m_pInfo );
		}
	}

	return 0;
}

void CNpcScript::CreateGuild()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->CreateGuild( m_pInfo );
		}
	}
}

//void CNpcScript::DestroyGuild()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->DestroyGuild( m_pInfo );
//		}
//	}
//}

void CNpcScript::PlaySound( DWORD szSound ) // PlaySound( "vocNpc-open1.wav" );
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			LPCTSTR lpszSound = GetDialogText( szSound );
			if( lpszSound == NULL )
			{
				return;
			}

			pTable->PlaySound( m_pInfo, lpszSound );
		}
	}
}

//int CNpcScript::GetLang()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetLang( m_pInfo );
//		}
//	}
//
//	return -1;
//}

//void CNpcScript::Run( DWORD szKey, int n ) // Run("Key_End", 0);	          
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			LPCTSTR lpszKey = GetDialogText( szKey );
//			if( lpszKey == NULL )
//			{
//				return;
//			}
//
//			pTable->Run( m_pInfo, lpszKey, n );
//		}
//	}
//}

//int CNpcScript::GetQuestId() // if( GetQuestId() == 500 )
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetQuestId( m_pInfo );
//		}
//	}
//
//	return 0;
//}

// BeginQuest( QUEST2_HEROWAY );
//void CNpcScript::BeginQuest( int nQuest ) 
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->BeginQuest( m_pInfo, nQuest );
//		}
//	}
//}

// EndQuest( QUEST2_HEROMIND );
//void CNpcScript::EndQuest( int nQuest ) 
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			pTable->EndQuest( m_pInfo, nQuest );
//		}
//	}
//}

// AddQuestKey( GetQuestId(), "Quest1_Begin" );
//void CNpcScript::AddQuestKey( int nQuest, DWORD szKey )   
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			LPCTSTR lpszKey = GetDialogText( szKey );
//			if( lpszKey == NULL )
//			{
//				return;
//			}
//
//			pTable->AddQuestKey( m_pInfo, nQuest, lpszKey );
//		}
//	}
//}

// SayQuest( QUEST_HERORAN_TRN1, QSAY_EXTRA01 );
void CNpcScript::SayQuest( int nQuest, int nDialog ) 
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->SayQuest( m_pInfo, nQuest, nDialog );
		}
	}
}

// PrintSystemMessage(TID_GAME_LACKSPACE);
void CNpcScript::PrintSystemMessage( int nID ) 
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->PrintSystemMessage( m_pInfo, nID );
		}
	}
}

//int CNpcScript::GetDay()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetDay( m_pInfo );
//		}
//	}
//
//	return 0;
//}

//int CNpcScript::GetMin()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetMin( m_pInfo );
//		}
//	}
//
//	return 0;
//}
//int CNpcScript::GetHour()
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			return pTable->GetHour( m_pInfo );
//		}
//	}
//
//	return 0;
//}

// AddCondKey( "아버지의 짐","CARGO" );
void CNpcScript::AddCondKey( DWORD szWord, DWORD szKey )  
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			LPCTSTR lpszWord = GetDialogText( szWord );
			LPCTSTR lpszKey = GetDialogText( szKey );
			if( lpszWord == NULL || lpszKey == NULL )
			{
				return;
			}

			pTable->AddCondKey( m_pInfo, lpszWord, lpszKey, 0 );	// dwParam - always 0
		}
	}
}

int CNpcScript::GetEmptyInventoryNum( DWORD dwItemId )	// if( GetEmptyInventoryNum() < 1 )
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetEmptyInventoryNum( m_pInfo, dwItemId );
		}
	}

	return 0;
}

void CNpcScript::LaunchQuest() // LaunchQuest();
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->LaunchQuest( m_pInfo );
		}
	}
}

//int CNpcScript::GetParam1()
//{
//	// do nothing
//	return 0;
//}
//
//int CNpcScript::GetParam2()
//{
//	// do nothing
//	return 0;
//}
//
//int CNpcScript::GetParam3()
//{
//	// do nothing
//	return 0;
//}
//
//int CNpcScript::GetParam4()
//{
//	// do nothing
//	return 0;
//}

//void CNpcScript::Trace( DWORD szMsg )
//{
//	if( m_pInfo != NULL )
//	{
//		Functions* pTable = m_pInfo->GetFunctions();
//		if( pTable != NULL )
//		{
//			LPCTSTR lpszMsg = GetDialogText( szMsg );
//			if( lpszMsg == NULL )
//			{
//				return;
//			}
//
//			pTable->Trace( m_pInfo, lpszMsg );
//		}
//	}
//}

void CNpcScript::Exit()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->Exit( m_pInfo );
		}
	}
}

int CNpcScript::GetPlayerExpPercent()
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			return pTable->GetPlayerExpPercent( m_pInfo );
		}
	}

	return 0;
}

void CNpcScript::SetLevel( int nSetLevel )
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->SetLevel( m_pInfo , nSetLevel );
		}
	}
}

void CNpcScript::AddGPPoint( int nAddGPPoint )
{
	if( m_pInfo != NULL )
	{
		Functions* pTable = m_pInfo->GetFunctions();
		if( pTable != NULL )
		{
			pTable->AddGPPoint( m_pInfo , nAddGPPoint );
		}
	}
}