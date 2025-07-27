### 【1972】report all spot weld info 报告所有点焊信息

#### 代码

```cpp
    /*HEAD REPORT_ALL_SPOT_WELD_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_weld.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以总结如下：

1. UF_print_syslog是V18版本中新增的功能。

2. UF_print_syslog用于打印系统日志信息。

3. UF_print_syslog只应提供翻译，不需要其他废话。

综上所述，UF_print_syslog是V18版本中新增的系统日志打印功能，其翻译为“UF_print_syslog”。 */  
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
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)))  
        {  
            if (!UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
            {  
                UF_CALL(UF_UI_open_listing_window());  
                sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_free(type);  
            }  
            UF_free(name);  
        }  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
    static void report_spot_weld_info(tag_t spot_weld)  
    {  
        int  
            ii, jj,  
            *n_faces_in_each_set,  
            num_sets,  
            spot_count;  
        tag_t  
            **set_of_faces,  
            *spot_points,  
            top_body;  
        if (UF_CALL(UF_WELD_ask_spot_weld_info(spot_weld, &top_body, &spot_count,  
            &spot_points, &num_sets, &n_faces_in_each_set, &set_of_faces))) return;  
        WRITE_D(top_body);  
        WRITE_D(spot_count);  
        for (ii = 0; ii < spot_count; ii++)  
            WRITE_D(spot_points[ii]);  
        UF_free(spot_points);  
        WRITE_D(num_sets);  
        for (ii = 0; ii < num_sets; ii++)  
        {  
            WRITE_D(n_faces_in_each_set[ii]);  
            for (jj = 0; jj < n_faces_in_each_set[ii]; jj++)  
            {  
                WRITE_D(set_of_faces[ii][jj]);  
                WRITE_L(UF_ASSEM_is_occurrence(set_of_faces[ii][jj]));  
            }  
        }  
        UF_free(n_faces_in_each_set);  
        UF_free_string_array(num_sets, (char **)set_of_faces);  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void report_spot_face_data(tag_t spot_weld)  
    {  
        int  
            ii, jj,  
            num_sets,  
            spot_count;  
        UF_WELD_spot_info_t  
            *spot_info;  
        if (UF_CALL(UF_WELD_ask_spot_face_data(spot_weld, &num_sets, &spot_count,  
            &spot_info))) return;  
        WRITE_D(num_sets);  
        WRITE_D(spot_count);  
        for (ii = 0; ii < spot_count; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(spot_info[ii].spot_point);  
            for (jj = 0; jj < num_sets; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(spot_info[ii].face_loc[jj].face_tag);  
                WRITE_L(UF_ASSEM_is_occurrence(spot_info[ii].face_loc[jj].  
                    face_tag));  
                WRITE3F(spot_info[ii].face_loc[jj].point);  
                WRITE3F(spot_info[ii].face_loc[jj].face_normal);  
                display_conehead(spot_info[ii].face_loc[jj].point,  
                    spot_info[ii].face_loc[jj].face_normal);  
            }  
        }  
        UF_CALL(UF_WELD_free_spot_data(spot_count, &spot_info));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_welds;  
        tag_t  
            part = UF_ASSEM_ask_work_part(),  
            *welds;  
        char  
            name[30];  
        UF_CALL(UF_WELD_locate_welds(part,  
            UF_WELD_SPOT_TYPE + UF_WELD_SPOT_NA_TYPE, &n_welds, &welds));  
        WRITE_D(n_welds);  
        for (ii = 0; ii < n_welds; ii++)  
        {  
            WRITE("\n");  
            sprintf(name, "welds[%d]", ii);  
            report_feature_name_and_type(name, welds[ii]);  
            report_spot_weld_info(welds[ii]);  
            report_spot_face_data(welds[ii]);  
        }  
        if (n_welds > 0) UF_free(welds);  
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

> 这是NX的二次开发代码，其主要功能是遍历并报告当前工作部件中的所有点焊特征的信息。
>
> 主要函数和步骤包括：
>
> 1. ufusr函数：这是NX二次开发的入口函数，首先调用UF_initialize()初始化NX环境，然后调用do_it()函数执行主体功能，最后调用UF_terminate()终止NX环境。
> 2. do_it函数：遍历当前工作部件中的所有点焊特征，并报告每个点焊特征的信息。首先调用UF_WELD_locate_welds()获取点焊特征数组，然后遍历数组，对每个点焊特征调用report_feature_name_and_type()报告名称和类型，report_spot_weld_info()报告点焊信息，report_spot_face_data()报告点焊面信息。
> 3. report_feature_name_and_type函数：报告一个特征的特征名称和类型。
> 4. report_spot_weld_info函数：报告一个点焊特征的基础信息，包括上件体、焊点数、焊点坐标、焊面组数、每组焊面数等。
> 5. report_spot_face_data函数：报告一个点焊特征的面信息，包括面坐标、法向量等，并在图形窗口中显示一个锥头。
> 6. 其他辅助函数：如WRITE系列宏，用于在列表窗口中输出信息。
> 7. ufusr_ask_unload函数：当用户卸载该二次开发程序时，立即卸载。
>
> 综上，该代码通过遍历点焊特征并报告详细信息，帮助用户了解当前部件中的点焊情况。
>
