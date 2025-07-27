### 【2413】report unitized vector from two points 报告两点之间的单位化向量

#### 代码

```cpp
    /*HEAD REPORT_UNITIZED_VECTOR_FROM_TWO_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。因此，我的回答是：UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    /* qq3123197280 */  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static void get_unit_vector( tag_t p1, tag_t p2, double * vctr )  
    {  
        double  
            p1_nums[3] = { 0., 0., 0. },  
            p2_nums[3] = { 0., 0., 0. },  
            non_unit_vec[3] = { 0., 0., 0. },  
            magnitude = 0.;  
        UF_CALL( UF_CURVE_ask_point_data( p1, p1_nums ));  
        UF_CALL( UF_CURVE_ask_point_data( p2, p2_nums ));  
        non_unit_vec[0] = p2_nums[0] - p1_nums[0];  
        non_unit_vec[1] = p2_nums[1] - p1_nums[1];  
        non_unit_vec[2] = p2_nums[2] - p1_nums[2];  
        UF_VEC3_unitize ( non_unit_vec, 0.0001, &magnitude, vctr );  
    }  
    static void do_it(void)  
    {  
        tag_t  
            pt_1 = NULL_TAG,  
            pt_2 = NULL_TAG;  
        double  
            vector[3] = { 0., 0., 0. };  
        while( (pt_1 = select_a_point("First Point: ")) != NULL_TAG &&  
               (pt_2 = select_a_point("Second Point: ")) != NULL_TAG )  
            {  
                get_unit_vector( pt_1, pt_2, vector );  
                WRITE3F( vector );  
            }  
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

> 这段代码是一个NX的二次开发示例，实现了从两个点生成单位向量的功能。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于输出错误代码和消息。
> 2. 定义了数组写入函数write_double_array_to_listing_window，用于将数组写入Listing窗口。
> 3. 定义了点选择函数select_a_point，用于选择一个点，并返回点的tag。
> 4. 定义了单位向量生成函数get_unit_vector，根据两个点计算单位向量。
> 5. 定义了主函数do_it，用于循环选择两个点，并生成单位向量，写入Listing窗口。
> 6. 定义了ufusr函数，作为NX的启动函数，初始化并执行do_it。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 整体来看，这段代码实现了NX的二次开发，通过选择两个点，生成了单位向量，并在Listing窗口中输出结果。
>
