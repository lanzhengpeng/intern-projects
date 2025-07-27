### 【2756】set drf lettering preference 设置图纸标注偏好

#### 代码

```cpp
    /*HEAD SET_DRF_LETTERING_PREFERENCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_ugfont.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18 */  
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
        tag_t  
            fte = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        int  
            index,  
            mpi[100];  
        char  
            font_name[] = "aurora",  
            diameter[27],  
            radius[27];  
        double  
            mpr[70];  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));  
        UF_CALL( UF_UGFONT_add_font( fte, &index, font_name));  
            printf("Aurora font assigned to index: %d\n", index);  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        mpi[88] = index;  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
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

> 这段代码是用于NX的二次开发，其主要功能是设置NX的绘图字体偏好。
>
> 代码的主要步骤包括：
>
> 1. 包含了必要的NX二次开发头文件。
> 2. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 3. 定义了一个do_it函数，用于实现设置字体的逻辑。
> 4. 在do_it函数中，首先获取当前活动部件，并循环查找字体表对象。
> 5. 使用UF_UGFONT_add_font函数向字体表中添加一个名为"Aurora"的字体，并获取其索引。
> 6. 使用UF_DRF_ask_preferences函数获取当前的绘图参数。
> 7. 将"Aurora"字体的索引赋值给绘图参数数组中的字体索引位置。
> 8. 使用UF_DRF_set_preferences函数将修改后的绘图参数设置回去，从而实现了设置绘图字体为"Aurora"。
> 9. ufusr函数用于初始化和调用do_it函数。
> 10. ufusr_ask_unload函数用于卸载二次开发程序。
>
> 总体来说，这段代码通过二次开发接口，实现了在NX中设置绘图字体的功能。
>
