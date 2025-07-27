### 【2813】set view matrix by vector 设置视图矩阵

#### 代码

```cpp
    /*HEAD SET_VIEW_MATRIX_BY_VECTOR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static void do_it(void)  
    {  
        double  vector[9], matrix[9];  
        double  origin[3], vec_dir[3];  
        int     resp, mode = UF_UI_NO_DEFAULT;  
        while( !(UF_CALL(UF_UI_specify_vector( "Select vector for view orientation",  
                        &mode, UF_UI_DISP_TEMP_VECTOR, vec_dir, origin, &resp)))  
                && (resp == UF_UI_OK))  
        {  
            UF_CALL(UF_MTX3_initialize_z( vec_dir, vector));  
            UF_MTX3_x_vec( vector, matrix);  
            UF_MTX3_y_vec( vector, &matrix[3]);  
            resp = uc6434( "", 4, NULL_TAG, matrix );  
            if(resp != 0) printf("Response from uc6434: %d\n", resp);  
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

> 这段NX二次开发代码的主要功能是设置视图矩阵以指定方向。以下是代码的关键点：
>
> 1. 引入了必要的头文件，如uf.h、uf_ui.h、uf_mtx.h和uf_view.h。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误。
> 3. 定义了report_error函数，用于在出错时打印错误信息到系统日志和列表窗口。
> 4. 定义了do_it函数，用于获取用户选择的向量，并据此计算视图矩阵。
> 5. 在do_it函数中，使用UF_UI_specify_vector函数让用户选择一个向量，以指定视图方向。
> 6. 使用UF_MTX3相关函数计算基于向量方向的视图矩阵。
> 7. ufusr函数是二次开发程序的入口函数，首先调用UF_initialize初始化环境，然后调用do_it执行主体功能，最后调用UF_terminate终止环境。
> 8. ufusr_ask_unload函数用于指定二次开发程序卸载模式为立即卸载。
>
> 总的来说，这段代码通过让用户选择一个向量来指定视图方向，并据此计算视图矩阵，实现了视图方向的灵活设置。
>
