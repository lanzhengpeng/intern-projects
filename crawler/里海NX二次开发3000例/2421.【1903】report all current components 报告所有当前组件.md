### 【1903】report all current components 报告所有当前组件

#### 代码

```cpp
    /*HEAD REPORT_ALL_CURRENT_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    #define WRITE3F(X) (write_double_array(#X, X, 1, 3))  
    #define WRITE9F(X) (write_double_array(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array(#X, X, 4, 4))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_current_cset(void)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, "CurrentComponents",  
            UF_component_set_type, FALSE, &cset));  
        return cset;  
    }  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_double_array(char *name, double *array, int rows, int cols)  
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
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
    static logical report_component_data(tag_t comp, void *user_data)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform)))  
        {  
            WRITE_S(part_name);  
            WRITE_S(refset_name);  
            WRITE_S(instance_name);  
            WRITE3F(origin);  
            WRITE9F(csys_matrix);  
            WRITE16F((double*)transform);  
            UF_CALL(UF_UI_write_listing_window("\n"));  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            current_cset = ask_current_cset();  
        UF_CALL(UF_ASSEM_apply_to_cset(current_cset,report_component_data,NULL));  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了报告错误函数report_error，用于在发生UF函数调用错误时输出错误信息和调用信息。
> 2. 定义了ask_current_cset函数，用于获取当前显示组件集。
> 3. 定义了write_string_to_listing_window和write_double_array函数，用于向Listing窗口输出字符串和数组。
> 4. 定义了report_component_data函数，用于遍历组件集中的每个组件，并输出其数据到Listing窗口。
> 5. 定义了do_it函数，用于获取当前组件集，并遍历其中的每个组件，输出其数据。
> 6. 定义了ufusr函数，作为NX二次开发的入口函数，初始化NX，调用do_it函数，然后终止NX。
> 7. 定义了ufusr_ask_unload函数，用于设置用户自定义的卸载策略。
>
> 总的来说，这段代码通过二次开发实现了遍历当前组件集，输出每个组件的详细信息的功能，包括组件名称、参考集名称、实例名称、原点、坐标系矩阵和变换矩阵等。
>
