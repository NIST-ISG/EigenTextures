//---------------------------------------------------------------------------
#include<iostream>
#include<istream>
#include<fstream>
#include<cmath>
#include<list>
#include<string>

#include "parameter_classes.h"

using namespace std;

//---------------------------------------------------------------------------

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

artif_im_param::artif_im_param()
{
	int im_dim = 0;    
	int* im_size = NULL;
	int num_saltpepper_param_within = 0;
	int num_saltpepper_param = 0;
	int** saltpepper_param = NULL;
	int num_Gauss_param_within = 0;
	int num_Gauss_param = 0;
	double** Gauss_param = NULL;
	int num_kern_param = 0;
	int** kern_param = NULL;
	int num_angs_kern = 0;
	double* ang_kern = NULL;
}

//---------------------------------------------------------------------------

artif_im_param::artif_im_param(int im_dim_inp, int* im_size_inp, int num_saltpepper_param_within_inp, int num_saltpepper_param_inp, 
	int** saltpepper_param_inp, int num_Gauss_param_within_inp, int num_Gauss_param_inp, double** Gauss_param_inp, int num_kern_param_inp, 
	int** kern_param_inp, int num_angs_kern_inp, double* ang_kern_inp)
{
    im_dim = im_dim_inp;
	im_size = im_size_inp;
	num_saltpepper_param_within = num_saltpepper_param_within_inp;
	num_saltpepper_param = num_saltpepper_param_inp;
	saltpepper_param = saltpepper_param_inp;
	num_Gauss_param_within = num_Gauss_param_within_inp;
	num_Gauss_param = num_Gauss_param_inp;
	Gauss_param = Gauss_param_inp;
	num_kern_param = num_kern_param_inp;
	kern_param = kern_param_inp;
	num_angs_kern = num_angs_kern_inp;
	ang_kern = ang_kern_inp;
}

//---------------------------------------------------------------------------

artif_im_param::artif_im_param(const artif_im_param& artif_im_param_or)
{
	im_dim = artif_im_param_or.im_dim;
	im_size = artif_im_param_or.im_size;
	num_saltpepper_param_within = artif_im_param_or.num_saltpepper_param_within;
	num_saltpepper_param = artif_im_param_or.num_saltpepper_param;
	saltpepper_param = artif_im_param_or.saltpepper_param;
	num_Gauss_param_within = artif_im_param_or.num_Gauss_param_within;
	num_Gauss_param = artif_im_param_or.num_Gauss_param;
	Gauss_param = artif_im_param_or.Gauss_param;
	num_kern_param = artif_im_param_or.num_kern_param;
	kern_param = artif_im_param_or.kern_param;
	num_angs_kern = artif_im_param_or.num_angs_kern;
	ang_kern = artif_im_param_or.ang_kern;
}

//---------------------------------------------------------------------------
artif_im_param::~artif_im_param()
{
    im_dim = 0;    
	delete im_size;
	for (int i = 0; i < num_saltpepper_param; i++)
    { delete *(saltpepper_param + i); }
    delete saltpepper_param;
	num_saltpepper_param = 0;
	num_saltpepper_param_within = 0;
	for (int i = 0; i < num_Gauss_param; i++)
    { delete *(Gauss_param + i); }
    delete Gauss_param;
	num_Gauss_param = 0;
	num_Gauss_param_within = 0;
	for (int i = 0; i < num_kern_param; i++)
    { delete *(kern_param + i); }
    delete kern_param;
	num_kern_param = 0;
	num_angs_kern = 0;
	delete ang_kern;	
}

//---------------------------------------------------------------------------
void artif_im_param::set_im_dim(int im_dim_inp) { im_dim = im_dim_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_im_dim(int& im_dim_out) { im_dim_out = im_dim; }
//---------------------------------------------------------------------------
void artif_im_param::set_im_size(int* im_size_inp) { im_size = im_size_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_im_size(int*& im_size_out) { im_size_out = im_size; }
//---------------------------------------------------------------------------
void artif_im_param::set_num_saltpepper_param_within(int num_saltpepper_param_within_inp) 
{ num_saltpepper_param_within = num_saltpepper_param_within_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_num_saltpepper_param_within(int& num_saltpepper_param_within_out) 
{ num_saltpepper_param_within_out = num_saltpepper_param_within; }
//---------------------------------------------------------------------------
void artif_im_param::set_num_saltpepper_param(int num_saltpepper_param_inp) { num_saltpepper_param = num_saltpepper_param_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_num_saltpepper_param(int& num_saltpepper_param_out) { num_saltpepper_param_out = num_saltpepper_param; }
//---------------------------------------------------------------------------
void artif_im_param::set_saltpepper_param(int** saltpepper_param_inp) { saltpepper_param = saltpepper_param_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_saltpepper_param(int**& saltpepper_param_out) { saltpepper_param_out = saltpepper_param; }
//---------------------------------------------------------------------------
void artif_im_param::set_num_Gauss_param(int num_Gauss_param_inp) { num_Gauss_param = num_Gauss_param_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_num_Gauss_param(int& num_Gauss_param_out) { num_Gauss_param_out = num_Gauss_param; }
//---------------------------------------------------------------------------
void artif_im_param::set_num_Gauss_param_within(int num_Gauss_param_within_inp) 
{ num_Gauss_param_within = num_Gauss_param_within_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_num_Gauss_param_within(int& num_Gauss_param_within_out) 
{ num_Gauss_param_within_out = num_Gauss_param_within; }
//---------------------------------------------------------------------------
void artif_im_param::set_Gauss_param(double** Gauss_param_inp) { Gauss_param = Gauss_param_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_Gauss_param(double**& Gauss_param_out) { Gauss_param_out = Gauss_param; }
//---------------------------------------------------------------------------
void artif_im_param::set_num_kern_param(int num_kern_param_inp) { num_kern_param = num_kern_param_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_num_kern_param(int& num_kern_param_out) { num_kern_param_out = num_kern_param; }
//---------------------------------------------------------------------------
void artif_im_param::set_kern_param(int** kern_param_inp) { kern_param = kern_param_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_kern_param(int**& kern_param_out) { kern_param_out = kern_param; }
//---------------------------------------------------------------------------
void artif_im_param::set_num_angs_kern(int num_angs_kern_inp) { num_angs_kern = num_angs_kern_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_num_angs_kern(int& num_angs_kern_out) { num_angs_kern_out = num_angs_kern; }
//---------------------------------------------------------------------------
void artif_im_param::set_ang_kern(double* ang_kern_inp) { ang_kern = ang_kern_inp; }
//---------------------------------------------------------------------------
void artif_im_param::get_ang_kern(double*& ang_kern_out) { ang_kern_out = ang_kern; }


//---------------------------------------------------------------------------

text_mod_param::text_mod_param(int im_dim_inp, int* im_size_inp, int* kernel_size_inp, int* inset_size_inp, double perc_val_inp, 
	int* incr_decr_inp)
{
    im_dim = im_dim_inp;
	im_size = im_size_inp;
	kernel_size = kernel_size_inp;
	inset_size = inset_size_inp;
	perc_val = perc_val_inp;
	incr_decr = incr_decr_inp;
}

//---------------------------------------------------------------------------

text_mod_param::text_mod_param(const text_mod_param& text_mod_param_or)
{
	im_dim = text_mod_param_or.im_dim;
	im_size = text_mod_param_or.im_size;
	kernel_size = text_mod_param_or.kernel_size;
	inset_size = text_mod_param_or.inset_size;
	perc_val = text_mod_param_or.perc_val;
	incr_decr = text_mod_param_or.incr_decr;
}

//---------------------------------------------------------------------------
text_mod_param::~text_mod_param()
{
    im_dim = 0; 
	delete im_size; 
	delete kernel_size; 
	delete inset_size; 
	perc_val = 0;
	delete incr_decr;
}
//---------------------------------------------------------------------------
void text_mod_param::set_im_dim(int im_dim_inp) { im_dim = im_dim_inp; }
//---------------------------------------------------------------------------
void text_mod_param::get_im_dim(int& im_dim_out) { im_dim_out = im_dim; }
//---------------------------------------------------------------------------
void text_mod_param::set_im_size(int* im_size_inp) { im_size = im_size_inp; }
//---------------------------------------------------------------------------
void text_mod_param::get_im_size(int*& im_size_out) { im_size_out = im_size; }
//---------------------------------------------------------------------------
void text_mod_param::set_kernel_size(int* kernel_size_inp) { kernel_size = kernel_size_inp; }
//---------------------------------------------------------------------------
void text_mod_param::get_kernel_size(int*& kernel_size_out) { kernel_size_out = kernel_size; }
//---------------------------------------------------------------------------
void text_mod_param::set_inset_size(int* inset_size_inp) { inset_size = inset_size_inp; }
//---------------------------------------------------------------------------
void text_mod_param::get_inset_size(int*& inset_size_out) { inset_size_out = inset_size; }
//---------------------------------------------------------------------------
void text_mod_param::set_perc_val(double perc_val_inp) { perc_val = perc_val_inp; }
//---------------------------------------------------------------------------
void text_mod_param::get_perc_val(double& perc_val_out) { perc_val_out = perc_val; }
//---------------------------------------------------------------------------
void text_mod_param::set_incr_decr(int* incr_decr_inp) { incr_decr = incr_decr_inp; }
//---------------------------------------------------------------------------
void text_mod_param::get_incr_decr(int*& incr_decr_out) { incr_decr_out = incr_decr; }
//---------------------------------------------------------------------------

Haralick_param::Haralick_param(int offset_inp, double ang_text_inp, int num_bins_inp, double max_val_inp, double min_val_inp,
	int par2_yn_inp, int num_offset_inp, int* offset_list_inp, double angoffset_inp, double minang_inp, double maxang_inp, 
	int num_bins2_inp, double max_val2_inp, double min_val2_inp, int tile_rows_inp, int tile_cols_inp)
{
    offset = offset_inp;
	ang_text = ang_text_inp;
	num_bins = num_bins_inp;
	max_val = max_val_inp;
	min_val = min_val_inp;
	par2_yn = par2_yn_inp;
	num_offset = num_offset_inp;
	offset_list = offset_list_inp;
	angoffset = angoffset_inp;
	minang = minang_inp;
	maxang = maxang_inp;
	num_bins2 = num_bins2_inp; 
	max_val2 = max_val2_inp; 
	min_val2 = min_val2_inp;
	tile_rows = tile_rows_inp;
	tile_cols = tile_cols_inp;
}

//---------------------------------------------------------------------------
Haralick_param::Haralick_param(const Haralick_param& Haralick_param_or)
{
	offset = Haralick_param_or.offset;
	ang_text = Haralick_param_or.ang_text;
	num_bins = Haralick_param_or.num_bins;
	max_val = Haralick_param_or.max_val;
	min_val = Haralick_param_or.min_val;
	par2_yn = Haralick_param_or.par2_yn;
	num_offset = Haralick_param_or.num_offset;
	offset_list = Haralick_param_or.offset_list;
	angoffset = Haralick_param_or.angoffset;
	minang = Haralick_param_or.minang;
	maxang = Haralick_param_or.maxang;
	num_bins2 = Haralick_param_or.num_bins2; 
	max_val2 = Haralick_param_or.max_val2; 
	min_val2 = Haralick_param_or.min_val2;
	tile_rows = Haralick_param_or.tile_rows;
	tile_cols = Haralick_param_or.tile_cols;
}

//---------------------------------------------------------------------------
Haralick_param::~Haralick_param()
{
    offset = 0; 
	ang_text = 0;
	num_bins = 0;
	max_val = 0;
	min_val = 0;
	par2_yn = 0;
	num_offset = 0;
	delete offset_list;
	angoffset = 0;
	minang = 0;
	maxang = 0;
	num_bins2 = 0; 
	max_val2 = 0; 
	min_val2 = 0;
	tile_rows = 0;
	tile_cols = 0;
}

//---------------------------------------------------------------------------
void Haralick_param::set_offset(int offset_inp) { offset = offset_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_offset(int& offset_out) { offset_out = offset; }
//---------------------------------------------------------------------------
void Haralick_param::set_ang_text(double ang_text_inp) { ang_text = ang_text_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_ang_text(double& ang_text_out) { ang_text_out = ang_text; }
//---------------------------------------------------------------------------
void Haralick_param::set_num_bins(int num_bins_inp) { num_bins = num_bins_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_num_bins(int& num_bins_out) { num_bins_out = num_bins; }
//---------------------------------------------------------------------------
void Haralick_param::set_max_val(double max_val_inp) { max_val = max_val_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_max_val(double& max_val_out) { max_val_out = max_val; }
//---------------------------------------------------------------------------
void Haralick_param::set_min_val(double min_val_inp) { min_val = min_val_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_min_val(double& min_val_out) { min_val_out = min_val; }
//---------------------------------------------------------------------------
void Haralick_param::set_par2_yn(int par2_yn_inp) { par2_yn = par2_yn_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_par2_yn(int& par2_yn_out) { par2_yn_out = par2_yn; }
//---------------------------------------------------------------------------
void Haralick_param::set_num_offset(int num_offset_inp) { num_offset = num_offset_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_num_offset(int& num_offset_out) { num_offset_out = num_offset; }
//---------------------------------------------------------------------------
void Haralick_param::set_offset_list(int* offset_list_inp) { offset_list = offset_list_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_offset_list(int*& offset_list_out) { offset_list_out = offset_list; }
//---------------------------------------------------------------------------
void Haralick_param::set_angoffset(double angoffset_inp) { angoffset = angoffset_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_angoffset(double& angoffset_out) { angoffset_out = angoffset; }
//---------------------------------------------------------------------------
void Haralick_param::set_minang(double minang_inp) { minang = minang_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_minang(double& minang_out) { minang_out = minang; }
//---------------------------------------------------------------------------
void Haralick_param::set_maxang(double maxang_inp) { maxang = maxang_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_maxang(double& maxang_out) { maxang_out = maxang; }
//---------------------------------------------------------------------------
void Haralick_param::set_num_bins2(int num_bins2_inp) { num_bins2 = num_bins2_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_num_bins2(int& num_bins2_out) { num_bins2_out = num_bins2; }
//---------------------------------------------------------------------------
void Haralick_param::set_max_val2(double max_val2_inp) { max_val2 = max_val2_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_max_val2(double& max_val2_out) { max_val2_out = max_val2; }
//---------------------------------------------------------------------------
void Haralick_param::set_min_val2(double min_val2_inp) { min_val2 = min_val2_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_min_val2(double& min_val2_out) { min_val2_out = min_val2; }
//---------------------------------------------------------------------------
void Haralick_param::set_tile_rows(int tile_rows_inp) { tile_rows = tile_rows_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_tile_rows(int& tile_rows_out) { tile_rows_out = tile_rows; }
//---------------------------------------------------------------------------
void Haralick_param::set_tile_cols(int tile_cols_inp) { tile_cols = tile_cols_inp; }
//---------------------------------------------------------------------------
void Haralick_param::get_tile_cols(int& tile_cols_out) { tile_cols_out = tile_cols; }
//---------------------------------------------------------------------------