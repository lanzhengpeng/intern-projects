### 【2186】report linear edge vector 报告线性边向量

#### 代码

```cpp
    /*HEAD REPORT_LINEAR_EDGE_VECTOR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中新增的函数。该函数的用途是打印系统日志信息。 */  
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
    /* qq3123197280 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a linear edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            type,  
            vertex_count;  
        tag_t  
            edge;  
        double   
            point1[ 3 ],   
            point2[ 3 ],  
            vector_length,  
            vector[ 3 ];  
        char  
            msg[128];  
        while ((edge = select_an_edge("Report linear edge vector")) != NULL_TAG)  
        {  
            WRITE_D(edge);  
            UF_CALL(UF_MODL_ask_edge_type(edge, &type));  
            WRITE_D(type);  
            if( type == UF_MODL_LINEAR_EDGE )  
            {  
                UF_CALL( UF_MODL_ask_edge_verts ( edge, point1, point2, &vertex_count ) );  
                vector[0] = point2[0] - point1[0];  
                vector[1] = point2[1] - point1[1];  
                vector[2] = point2[2] - point1[2];  
                vector_length = sqrt( vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);  
                vector[0] = vector[0]/vector_length;  
                vector[1] = vector[1]/vector_length;  
                vector[2] = vector[2]/vector_length;  
                sprintf( msg, "Vector: %f %f %f\n\n", vector[0], vector[1], vector[2]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
            }  
            else  
            {  
                sprintf( msg, "Edge is non-linear\n\n");  
                UF_CALL(UF_UI_write_listing_window(msg));  
            }  
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

> 这段代码是一个NX的二次开发示例，其主要功能是选择一个线性边，并报告该边的方向向量。以下是该代码的主要功能和流程：
>
> 1. 头文件包含：引入了必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在API调用出错时打印错误信息。
> 3. 选择边：定义了一个函数select_an_edge，用于弹出一个对话框让用户选择一个线性边，并返回边的tag。
> 4. 写日志函数：定义了两个函数write_integer_to_listing_window和write_string_to_listing_window，用于将整数和字符串写入日志窗口。
> 5. 报告向量：定义了一个函数do_it，用于循环选择边，并报告每个边的方向向量。首先判断边是否为线性边，如果是，则获取边的两个顶点坐标，计算方向向量，并打印到日志窗口。
> 6. ufusr主函数：这是二次开发用户入口函数，在这里调用do_it函数执行主功能。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 综上所述，该代码通过选择线性边，并报告边的方向向量，展示了NX二次开发中的错误处理、选择、日志写入等基本技巧。
>
