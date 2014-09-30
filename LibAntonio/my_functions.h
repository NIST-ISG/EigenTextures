#ifndef My_functionsH
#define My_functionsH
//---------------------------------------------------------------------------

using namespace cv;
using namespace std;

//---------------------------------------------------------------------------

int comput_dist_roi(int*& dist, int*& pix_val, Mat roi_curr, Mat* itext_mod_list, int mod_inset_rows, int mod_inset_cols, 
	Mat*& smaller_text_mod);
void find_min(int*& min_dist, int*& index_min, int* dist, int sz_dist, double perc, int& sz_index_min);
void assign_pixel_roi(Mat& roi_curr, int* pix_val, int index_min, Mat* smaller_text_mod, int which_transv);
void process_mod_border(bool& colpos1_lim, bool& colpos2_lim, bool& rowpos1_lim, bool& rowpos2_lim, int*& pos_col_transv, 
	int*& pos_row_transv, int pad_left, int pad_right, int pad_top, int pad_bottom, Mat*& synth_im_curr_pad, int num_col_roi, 
	int num_row_roi, int mod_inset_rows, int mod_inset_cols, Mat* itext_mod_list, int num_col_synth_im_curr_pad, 
	int num_row_synth_im_curr_pad, double perc_smaller_text_mod, bool colpos1_incr, bool colpos2_incr, bool rowpos1_incr, 
	bool rowpos2_incr, int& count_while);
void bubble_sort_int(int*& min_dist, int*& sorting_indexes, int sz_index_min);
Mat* get_co_occurrence_mat(int offset, double ang_text, int num_bins, double max_val, double min_val, Mat* im_inp);
void get_co_occurrence_mat_props(Mat* co_occ_mat, double& contrast, double& energy, double& homogeneity, double& correlation);
void get_min_from_vector(double* vector, int vector_sz, int& pos, double& val);
void get_min_from_vector(int* vector, int vector_sz, int& pos, int& val, bool& multiple_occ);
void get_max_from_vector(double* vector, int vector_sz, int& pos, double& val);
void get_max_from_vector(int* vector, int vector_sz, int& pos, int& val, bool& multiple_occ);
void get_mean_std_from_vector(double* vector, int vector_sz, double& mean, double& std);
void erase_el_from_vector(double* vector_in, double*& vector_out, int& vector_sz, int pos);
void erase_el_from_vector(int* vector_in, int*& vector_out, int& vector_sz, int pos);

#endif