### 【0655】create csys matching sketch csys 创建与草图坐标系匹配的坐标系

#### 代码

```cpp
    /*HEAD CREATE_CSYS_MATCHING_SKETCH_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            sketch = NULL_TAG,  
            new_csys_tag,  
            matrix_id = NULL_TAG;  
        double  
            mat_vals[9],  
            origin[3];  
        UF_SKET_info_t  
            sket_info;  
        while ((sketch = select_a_sketch("Select sketch")) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_sketch_info(sketch, &sket_info));  
            for (ii = 0; ii < 9; ii++)   
            {  
                mat_vals[ii] = sket_info.csys[ii];    
            }  
            for (ii = 0; ii < 3; ii++)   
            {  
                origin[ii] = sket_info.csys[ii + 9];      
            }  
            UF_CSYS_create_matrix( mat_vals, &matrix_id );  
            UF_CALL(UF_CSYS_create_csys(origin, matrix_id, &new_csys_tag));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open的二次开发示例，主要实现了以下功能：
>
> 1. 包含必要的NX Open头文件，包括UF、UI、SKET和CSYS模块。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息到系统日志和窗口。
> 3. 定义了一个选择草图的函数select_a_sketch，使用UI模块的UF_UI_select_sketch函数让用户选择一个草图。
> 4. 定义了一个do_it函数，用于创建一个与草图坐标系匹配的新坐标系：获取用户选择的草图信息，包括草图坐标系和原点。根据草图坐标系创建一个变换矩阵。根据草图原点和变换矩阵创建一个新的坐标系。
> 5. 获取用户选择的草图信息，包括草图坐标系和原点。
> 6. 根据草图坐标系创建一个变换矩阵。
> 7. 根据草图原点和变换矩阵创建一个新的坐标系。
> 8. 定义了ufusr函数，作为程序的入口点。在此函数中初始化NX Open，调用do_it函数，然后终止NX Open。
> 9. 定义了一个卸载函数ufusr_ask_unload，表示程序不需要延迟卸载。
>
> 总体来说，这段代码通过UI模块让用户选择草图，然后根据草图坐标系创建一个新坐标系，实现了草图坐标系的复制功能。
>
