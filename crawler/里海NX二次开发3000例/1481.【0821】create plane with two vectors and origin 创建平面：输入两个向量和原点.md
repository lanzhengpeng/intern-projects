### 【0821】create plane with two vectors and origin 创建平面：输入两个向量和原点

#### 代码

```cpp
    /*HEAD CREATE_PLANE_WITH_TWO_VECTORS_AND_ORIGIN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_mtx.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，UF_print_syslog 是 V18 新增的功能。

翻译如下：

注意：UF_print_syslog 是 V18 的新增功能。 */  
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
    static logical prompt_for_a_vector(char *prompt, double vec[3])  
    {  
        char  
            menu[3][16] = { "X", "Y", "Z" };  
        int  
            irc,  
            junk;  
        irc = uc1609(prompt, menu, 3, vec, &junk);  
        if (irc == 3 || irc == 4) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            new_plane = NULL_TAG;  
        double  
            vec1[3] = { 0., 0., 0. },      
            vec2[3] = { 0., 0., 0. },      
            vec3[3] = { 0., 0., 0. },      
            org[3] = { 0., 0., 0. },  
            mtx[9] = { 0., 0., 0.,  0., 0., 0.,  0., 0., 0.};      
        while( (prompt_for_a_vector( "First Vector: ( -X- )", vec1 ) == TRUE ) &&  
               (prompt_for_a_vector( "Second Vector: ( -Y- )", vec2 ) == TRUE ) &&   
               (prompt_for_a_vector( "Origin Point:", org ) == TRUE )   
          )  
          {  
            UF_MTX3_initialize( vec1, vec2, mtx );  
            vec3[0] = mtx[6];  
            vec3[1] = mtx[7];  
            vec3[2] = mtx[8];  
            UF_CALL( UF_MODL_create_plane( org, vec3, &new_plane ));  
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

> 这段代码是用于NX的二次开发，主要功能是创建一个平面。具体步骤如下：
>
> 1. 包含必要的头文件，如uf.h, uf_mtx.h, uf_modl.h等。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并报告错误。
> 3. 实现了report_error函数，用于输出错误信息。
> 4. 实现了prompt_for_a_vector函数，用于提示用户输入一个向量。
> 5. do_it函数是主体，循环提示用户输入两个向量和原点，然后使用UF_MTX3_initialize创建一个3x3矩阵，提取法向量，并调用UF_MODL_create_plane创建平面。
> 6. ufusr函数是NX二次开发的入口，初始化NX环境，调用do_it，然后终止环境。
> 7. ufusr_ask_unload函数用于卸载二次开发模块。
>
> 总的来说，这段代码通过交互方式获取用户输入的两个向量和原点，创建一个平面。
>
