### 【1718】orient view to sketch csys 视图对齐到草图坐标系

#### 代码

```cpp
    /*HEAD ORIENT_VIEW_TO_SKETCH_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 新增的功能。因此，我的回答是：UF_print_syslog 是 V18 版本新增的功能。 */  
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
            temp_csys,  
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
            UF_CALL(UF_CSYS_create_temp_csys (origin, matrix_id, &temp_csys));  
            UF_CALL(UF_CSYS_set_wcs(temp_csys));   
            UF_CALL(uc6434("", 2, NULL_TAG, NULL));  
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

> 这段代码是一个NX二次开发示例，用于将草图的坐标系设置为当前坐标系。具体来说：
>
> 1. 函数报告错误：定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 选择草图：定义了一个函数select_a_sketch，用于让用户选择一个草图实体，并返回其tag。
> 3. 主体函数do_it：循环调用select_a_sketch获取用户选择的草图，然后获取草图的坐标系信息，并创建一个临时坐标系设置为当前坐标系。
> 4. ufusr函数：NX二次开发程序的入口函数，在这里调用do_it函数执行主体功能。
> 5. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于卸载二次开发程序。
>
> 通过这段代码，用户可以方便地将任意草图的坐标系设置为当前坐标系，便于进行后续操作。
>
