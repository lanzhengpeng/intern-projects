### 【2303】report parent csys of wcs 查找工作坐标系的父坐标系

#### 代码

```cpp
    /*HEAD REPORT_PARENT_CSYS_OF_WCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。在V18版本中，UF_print_syslog是一个新的功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static tag_t find_parent_csys_of_wcs( tag_t wcs )  
    {  
    /*  
        The WCS is a type 45, subtype 1.  
        A normal CSYS entity is a type 45, subtype 0.  
        Pass in a WCS, and if it is based on an existing normal CSYS, entity,  
        the tag for the csys entity will be returned.  
    */    int  
            type = 0,  
            subtype = 0;  
        tag_t  
            wcs_mx_id = NULL_TAG,  
            csys_mx_id = NULL_TAG,  
            csys_tag = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            wcs_origin[3],  
            csys_origin[3],  
            wcs_mx[9],  
            csys_mx[9];  
        UF_CALL(UF_CSYS_ask_csys_info( wcs, &wcs_mx_id, wcs_origin ));   
        UF_CALL(UF_CSYS_ask_matrix_values( wcs_mx_id, wcs_mx ));  
        while(!UF_CALL(UF_OBJ_cycle_objs_in_part( part,   
                UF_coordinate_system_type, &csys_tag )) && NULL_TAG != csys_tag )  
            {  
                UF_CALL( UF_OBJ_ask_type_and_subtype( csys_tag, &type, &subtype ));  
                if( subtype == 1 ) break;  
                UF_CALL(UF_CSYS_ask_csys_info( csys_tag, &csys_mx_id, csys_origin ));   
                UF_CALL(UF_CSYS_ask_matrix_values( csys_mx_id, csys_mx ));  
                if(  
                  ( wcs_origin[0] == csys_origin[0] ) &&  
                  ( wcs_origin[1] == csys_origin[1] ) &&  
                  ( wcs_origin[2] == csys_origin[2] ) &&  
                  ( wcs_mx[0] == csys_mx[0] ) &&  
                  ( wcs_mx[1] == csys_mx[1] ) &&  
                  ( wcs_mx[2] == csys_mx[2] ) &&  
                  ( wcs_mx[3] == csys_mx[3] ) &&  
                  ( wcs_mx[4] == csys_mx[4] ) &&  
                  ( wcs_mx[5] == csys_mx[5] ) &&  
                  ( wcs_mx[6] == csys_mx[6] ) &&  
                  ( wcs_mx[7] == csys_mx[7] ) &&  
                  ( wcs_mx[8] == csys_mx[8] )  
                  ) return( csys_tag );  
            }  
            return( NULL_TAG );  
    }  
    static void do_it(void)  
    {  
        tag_t  
            wcs_tag = NULL_TAG,  
            csys_tag = NULL_TAG;  
        UF_CALL( UF_CSYS_ask_wcs ( &wcs_tag ));      
        WRITE_D( wcs_tag );  
        csys_tag = find_parent_csys_of_wcs( wcs_tag );  
        WRITE_D( csys_tag );         
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

> 这是NX的二次开发代码，主要功能是查找工作坐标系(WCS)的父坐标系。以下是代码的简要介绍：
>
> 1. 定义了UF_CALL宏，用于调用UF函数并报告错误。
> 2. 定义了WRITE_D宏，用于将整数值输出到列表窗口。
> 3. 定义了find_parent_csys_of_wcs函数，用于查找给定WCS的父坐标系。它会遍历当前零件中的所有坐标系，比较WCS和每个坐标系的参数，如果找到匹配的坐标系，则返回该坐标系的tag。
> 4. 定义了do_it函数，用于获取当前WCS的tag，并调用find_parent_csys_of_wcs函数查找父坐标系，并将结果输出到列表窗口。
> 5. 定义了ufusr函数，这是NX二次开发程序的入口函数。在函数内部，首先初始化UF，然后调用do_it函数执行主要功能，最后终止UF。
> 6. 定义了ufusr_ask_unload函数，用于告诉NX立即卸载该二次开发程序。
>
> 综上所述，该代码的主要功能是遍历零件坐标系，查找与WCS参数匹配的坐标系，并输出结果。
>
