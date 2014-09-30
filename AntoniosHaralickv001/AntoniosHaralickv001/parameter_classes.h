#ifndef Parameter_classesH
#define Parameter_classesH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

class artif_im_param {

int im_dim;    
int* im_size;
int num_saltpepper_param_within;
int num_saltpepper_param;
int** saltpepper_param;
int num_Gauss_param_within;
int num_Gauss_param;
double** Gauss_param;
int num_kern_param;
int** kern_param;
int num_angs_kern;
double* ang_kern;

public:

artif_im_param::artif_im_param();    	
artif_im_param::artif_im_param(int im_dim_inp, int* im_size_inp, int num_saltpepper_param_within_inp, int num_saltpepper_param_inp, 
int** saltpepper_param_inp, int num_Gauss_param_within_inp, int num_Gauss_param_inp, double** Gauss_param_inp, int num_kern_param_inp, 
int** kern_param_inp, int num_angs_kern_inp, double* ang_kern_inp);        
artif_im_param::artif_im_param(const artif_im_param& artif_im_param_or);
artif_im_param::~artif_im_param();       	
void artif_im_param::set_im_dim(int im_dim_inp);
void artif_im_param::get_im_dim(int& im_dim_out);
void artif_im_param::set_im_size(int* im_size_inp);
void artif_im_param::get_im_size(int*& im_size_out);
void artif_im_param::set_num_saltpepper_param_within(int num_saltpepper_param_within_inp);
void artif_im_param::get_num_saltpepper_param_within(int& num_saltpepper_param_within_out);
void artif_im_param::set_num_saltpepper_param(int num_saltpepper_param_inp);
void artif_im_param::get_num_saltpepper_param(int& num_saltpepper_param_out);
void artif_im_param::set_saltpepper_param(int** saltpepper_param_inp);
void artif_im_param::get_saltpepper_param(int**& saltpepper_param_out);
void artif_im_param::set_num_Gauss_param_within(int num_Gauss_param_within_inp);
void artif_im_param::get_num_Gauss_param_within(int& num_Gauss_param_within_out);
void artif_im_param::set_num_Gauss_param(int num_Gauss_param_inp);
void artif_im_param::get_num_Gauss_param(int& num_Gauss_param_out);
void artif_im_param::set_Gauss_param(double** Gauss_param_inp);
void artif_im_param::get_Gauss_param(double**& Gauss_param_out);
void artif_im_param::set_num_kern_param(int num_kern_param_inp);
void artif_im_param::get_num_kern_param(int& num_kern_param_out);
void artif_im_param::set_kern_param(int** kern_param_inp);
void artif_im_param::get_kern_param(int**& kern_param_out);
void artif_im_param::set_num_angs_kern(int num_angs_kern_inp);
void artif_im_param::get_num_angs_kern(int& num_angs_kern_out);
void artif_im_param::set_ang_kern(double* ang_kern_inp);
void artif_im_param::get_ang_kern(double*& ang_kern_out);
};

//---------------------------------------------------------------------------

class text_mod_param {

int im_dim;    
int* im_size;
int* kernel_size;
int* inset_size;
double perc_val;
int* incr_decr;

public:

text_mod_param::text_mod_param();    	
text_mod_param::text_mod_param(int im_dim_inp, int* im_size_inp, int* kernel_size_inp, int* inset_size_inp, double perc_val_inp, 
int* inc_decr_inp);        
text_mod_param::text_mod_param(const text_mod_param& text_mod_param_or);
text_mod_param::~text_mod_param();       	
void text_mod_param::set_im_dim(int im_dim_inp);
void text_mod_param::get_im_dim(int& im_dim_out);
void text_mod_param::set_im_size(int* im_size_inp);
void text_mod_param::get_im_size(int*& im_size_out);
void text_mod_param::set_kernel_size(int* kernel_size_inp);
void text_mod_param::get_kernel_size(int*& kernel_size_out);
void text_mod_param::set_inset_size(int* inset_size_inp);
void text_mod_param::get_inset_size(int*& inset_size_out);
void text_mod_param::set_perc_val(double perc_val_inp);
void text_mod_param::get_perc_val(double& perc_val_out);
void text_mod_param::set_incr_decr(int* incr_decr_inp);
void text_mod_param::get_incr_decr(int*& incr_decr_out);
};

//---------------------------------------------------------------------------

class Haralick_param {

int offset;    
double ang_text; 
int num_bins; 
double max_val; 
double min_val;
int par2_yn;
int num_offset;
int* offset_list;
double angoffset, minang, maxang;
int num_bins2; 
double max_val2; 
double min_val2;
int tile_rows;
int tile_cols;

public:

Haralick_param::Haralick_param();    	
Haralick_param::Haralick_param(int offset_inp, double ang_text_inp, int num_bins_inp, double max_val_inp, double min_val_inp, int par2_yn_inp, 
	int num_offset_inp, int* offset_list_inp, double angoffset_inp, double minang_inp, double maxang_inp, int num_bins2_inp, double max_val2_inp,  
	double min_val2_inp, int tile_rows_inp, int tile_cols_inp);      
Haralick_param::Haralick_param(const Haralick_param& Haralick_param_or);
Haralick_param::~Haralick_param();       	
void Haralick_param::set_offset(int offset_inp);
void Haralick_param::get_offset(int& offset_out);
void Haralick_param::set_ang_text(double ang_text_inp);
void Haralick_param::get_ang_text(double& ang_text_out);
void Haralick_param::set_num_bins(int num_bins_inp);
void Haralick_param::get_num_bins(int& num_bins_out);
void Haralick_param::set_max_val(double max_val_inp);
void Haralick_param::get_max_val(double& max_val_out);
void Haralick_param::set_min_val(double min_val_inp);
void Haralick_param::get_min_val(double& min_val_out);
void Haralick_param::set_par2_yn(int par2_yn_inp);
void Haralick_param::get_par2_yn(int& par2_yn_out);
void Haralick_param::set_num_offset(int num_offset_inp);
void Haralick_param::get_num_offset(int& num_offset_out);
void Haralick_param::set_offset_list(int* offset_list_inp);
void Haralick_param::get_offset_list(int*& offset_list_out);
void Haralick_param::set_angoffset(double angoffset_inp);
void Haralick_param::get_angoffset(double& angoffset_out);
void Haralick_param::set_minang(double minang_inp);
void Haralick_param::get_minang(double& minang_out);
void Haralick_param::set_maxang(double maxang_inp);
void Haralick_param::get_maxang(double& maxang_out);
void Haralick_param::set_num_bins2(int num_bins2_inp);
void Haralick_param::get_num_bins2(int& num_bins2_out);
void Haralick_param::set_max_val2(double max_val2_inp);
void Haralick_param::get_max_val2(double& max_val2_out);
void Haralick_param::set_min_val2(double min_val2_inp);
void Haralick_param::get_min_val2(double& min_val2_out);
void Haralick_param::set_tile_rows(int tile_rows_inp);
void Haralick_param::get_tile_rows(int& tile_rows_out);
void Haralick_param::set_tile_cols(int tile_cols_inp);
void Haralick_param::get_tile_cols(int& tile_cols_out);
};

//---------------------------------------------------------------------------

#endif
