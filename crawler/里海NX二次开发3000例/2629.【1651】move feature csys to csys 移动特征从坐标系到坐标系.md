### 【1651】move feature csys to csys 移动特征从坐标系到坐标系

#### 代码

```cpp
/*HEAD MOVE_FEATURE_CSYS_TO_CSYS CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_csys.h>
#include <uf_ui.h>
#include <uf_modl.h>
#include <uf_object_types.h>
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
             msg[133];
        sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog是V18版本新增的。 */
        UF_print_syslog(msg, FALSE);
        UF_print_syslog(err, FALSE);
        UF_print_syslog("\n", FALSE);
        UF_print_syslog(call, FALSE);
        UF_print_syslog(";\n", FALSE);
        if (!UF_UI_open_listing_window())
        {
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n");
        }
    }
    return(irc);
}
#define WRITE_F(X) (write_double_to_listing_window(#X, X))
static void write_double_to_listing_window(char *title, double n)
{
    char
        msg[MAX_LINE_SIZE+1];
    UF_CALL(UF_UI_open_listing_window());
    sprintf(msg, "%s = %f\n", title, n);
    UF_CALL(UF_UI_write_listing_window(msg));
}
/* qq3123197280 */
static uf_list_p_t make_a_list( int count, tag_t *item_array)
{
    uf_list_p_t 
        list;
    int
        ii;
    UF_CALL( UF_MODL_create_list( &list));
    for( ii = 0; ii < count; ii++)
        UF_CALL( UF_MODL_put_list_item( list, item_array[ii]));
    return( list);
}
/* qq3123197280 */
static int select_features(char *prompt, tag_t **feats)
{
    int
        cnt,
        resp;
    UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));
    if (resp == UF_UI_OK) return cnt;
    else return 0;
}
/* qq3123197280 */
static int mask_for_csys(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_coordinate_system_type, 0, 0 };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
/* qq3123197280 */
static tag_t select_a_csys(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",
        prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
static void do_it(void)
{
    int     
        cnt = 0,
        inx = 0;
    tag_t         
        ref_csys = NULL_TAG,
        dest_csys = NULL_TAG,
        ref_mtx_tag = NULL_TAG,
        dst_mtx_tag = NULL_TAG,
        *feats;
    double  
       ref_orig[3] = { 0., 0., 0. },
       dst_orig[3] = { 0., 0., 0. },
       dst_mtx[9] = { 1., 0., 0.,  0., 1., 0.,  0., 0., 1. },
       ref_mtx[9] = { 1., 0., 0.,  0., 1., 0.,  0., 0., 1. },
       real_data[2][12] = { 0., 0., 0.,  0., 0., 0.,  0., 0., 0.,  0., 0., 0.,
                            0., 0., 0.,  0., 0., 0.,  0., 0., 0.,  0., 0., 0. };
    uf_list_p_t 
        feat_list;
    cnt = select_features("Select features", &feats);
    feat_list = make_a_list( cnt, feats);
    ref_csys = select_a_csys( "Select Reference CSYS");
    if( NULL_TAG == ref_csys ) return;
    UF_CALL(UF_CSYS_ask_matrix_of_object( ref_csys, &ref_mtx_tag ));
    UF_CALL(UF_CSYS_ask_csys_info( ref_csys, &ref_mtx_tag, ref_orig )); 
    UF_CALL(UF_CSYS_ask_matrix_values ( ref_mtx_tag, ref_mtx ));
    real_data[0][0] = ref_orig[0];
    real_data[0][1] = ref_orig[1];
    real_data[0][2] = ref_orig[2];
    for( inx = 0; inx < 9; inx++ )
    {
        real_data[0][inx+3] = ref_mtx[inx];
    }
    for( inx = 0; inx < 12; inx++ ) WRITE_F(real_data[0][inx]);
    dest_csys = select_a_csys( "Select Destination CSYS");
    if( NULL_TAG == ref_csys ) return;
    UF_CALL(UF_CSYS_ask_matrix_of_object( dest_csys, &dst_mtx_tag ));
    UF_CALL(UF_CSYS_ask_csys_info( dest_csys, &dst_mtx_tag, dst_orig )); 
    UF_CALL(UF_CSYS_ask_matrix_values ( dst_mtx_tag, dst_mtx ));
    real_data[1][0] = dst_orig[0];
    real_data[1][1] = dst_orig[1];
    real_data[1][2] = dst_orig[2];
    for( inx = 0; inx < 9; inx++ )
    {
        real_data[1][inx+3] = dst_mtx[inx];
    }
    for( inx = 0; inx < 12; inx++ ) WRITE_F(real_data[1][inx]);
    UF_CALL( UF_MODL_move_feature( feat_list, CSYS_TO_CSYS, real_data));
    UF_CALL( UF_MODL_update());
    UF_free( feats);    
    UF_CALL( UF_MODL_delete_list( &feat_list ));
}
/* qq3123197280 */
void ufusr(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    do_it();
    UF_terminate();
}
int ufusr_ask_unload(void)
{
    return (UF_UNLOAD_IMMEDIATELY);
}

```

#### 代码解析

> 这段NX二次开发代码实现了将特征从一个坐标系移动到另一个坐标系的功能。
>
> 主要功能包括：
>
> 1. 特征选择：使用UF_UI_select_feature函数，让用户选择要移动的特征，并将选中的特征存储在特征列表中。
> 2. 坐标系选择：定义了mask_for_csys函数，允许用户选择坐标系。分别选择参考坐标系和目标坐标系。
> 3. 获取坐标系信息：使用UF_CSYS_ask_matrix_of_object、UF_CSYS_ask_csys_info和UF_CSYS_ask_matrix_values函数，获取参考坐标系和目标坐标系的原始点和变换矩阵。
> 4. 特征移动：调用UF_MODL_move_feature函数，将特征从参考坐标系移动到目标坐标系。移动时，需要提供参考坐标系和目标坐标系的原始点和变换矩阵。
> 5. 更新模型：调用UF_MODL_update函数，更新模型显示。
> 6. 错误处理：通过宏定义，在函数调用后添加错误处理，以打印错误信息。
> 7. 资源释放：在函数结束前，释放特征列表内存。
>
> 总体来说，这段代码实现了NX特征从一个坐标系到另一个坐标系的移动，同时包含了错误处理和资源释放。
>
