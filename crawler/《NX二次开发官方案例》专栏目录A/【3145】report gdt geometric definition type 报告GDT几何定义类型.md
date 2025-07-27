### 【3145】report gdt geometric definition type 报告GDT几何定义类型

#### 代码

```cpp
    /*HEAD REPORT_GDT_GEOMETRIC_DEFINITION_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_gdt.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #define WRITE(X)      (UF_UI_write_listing_window(X))  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_tolerance_feature(tag_t part, tag_t tol_feat)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tolerance_feature_type,  
                &tol_feat));  
            if (tol_feat)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(tol_feat, &type, &subtype));  
                if (subtype == UF_tolerance_feature_subtype) return tol_feat;  
            }  
        } while (tol_feat);  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            tol_feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_GDT_geometric_definition_t   
            *data;  
        while ((tol_feat = ask_next_tolerance_feature(part, tol_feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_GDT_ask_geometric_definition(tol_feat, &data));  
            WRITE_D( tol_feat );   
            WRITE("    UF_GDT_geometric_definition_type =  ");   
            switch(data->type)  
            {  
                case UF_GDT_ALL_GEOMETRY:  
                    WRITE_D( UF_GDT_ALL_GEOMETRY );  
                    break;  
                case UF_GDT_PLANAR_ORIENTATION:  
                    WRITE_D( UF_GDT_PLANAR_ORIENTATION );  
                    break;  
                case UF_GDT_PLANAR_CROSS_SECTION:  
                    WRITE_D( UF_GDT_PLANAR_CROSS_SECTION );  
                case UF_GDT_REGION:   
                    WRITE_D( UF_GDT_REGION );  
                    break;                   
                case UF_GDT_POINT:  
                    WRITE_D( UF_GDT_POINT );   
                    break;                       
                default:  
                break;  
            }          
            UF_free(data);  
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

> 这段代码是一个NX的二次开发示例，主要功能是遍历当前显示部件中的公差特征，并报告每个公差特征的几何定义类型。
>
> 主要步骤包括：
>
> 1. 包含必要的NX API头文件。
> 2. 定义错误报告函数report_error，用于打印错误信息。
> 3. 定义整数写入列表窗口函数write_integer_to_listing_window。
> 4. 定义获取下一个公差特征函数ask_next_tolerance_feature。
> 5. 主函数do_it中，首先获取当前显示部件，然后循环获取每个公差特征，并调用UF_GDT_ask_geometric_definition函数获取其几何定义数据，最后打印公差特征tag和几何定义类型。
> 6. ufusr函数是NX二次开发程序的入口，调用do_it函数执行主功能。
> 7. ufusr_ask_unload函数返回立即卸载，表示二次开发程序可以立即卸载。
>
> 该代码实现了遍历当前显示部件中的公差特征，并获取每个特征的几何定义类型，打印输出，是一个典型的NX二次开发示例。
>
