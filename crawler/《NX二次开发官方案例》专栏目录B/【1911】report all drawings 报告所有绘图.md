### 【1911】report all drawings 报告所有绘图

#### 代码

```cpp
    /*HEAD REPORT_ALL_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */       
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */       
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    /* qq3123197280 */       
    void report_drawing_info(tag_t drawing)  
    {  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            msg[300] = { "" };  
        UF_DRAW_info_t drawing_info;  
        UF_CALL(UF_OBJ_ask_name(drawing, dname));  
        UF_CALL(UF_DRAW_ask_drawing_info( drawing, &drawing_info));  
        sprintf( msg, "\n%s\n", dname);  
        WRITE( msg );  
        sprintf( msg,  "The value of the drawing info structure is:\n" );  
        WRITE( msg );  
        sprintf( msg,  "size_state:  %d\n", drawing_info.size_state );  
        WRITE( msg );  
        if(drawing_info.size_state == UF_DRAW_METRIC_SIZE )  
        {  
            sprintf( msg,  "standard METRIC size %d\n",  
                        drawing_info.size.metric_size_code );      
            WRITE( msg );  
        }  
        else if(drawing_info.size_state == UF_DRAW_ENGLISH_SIZE )  
        {  
            sprintf( msg,  "standard ENGLISH size %d\n",  
                        drawing_info.size.english_size_code );  
            WRITE( msg );  
        }  
        else  
        {  
            sprintf( msg, "CUSTOM size = %f,  %f\n",  
            drawing_info.size.custom_size[0],   
            drawing_info.size.custom_size[1] );  
            WRITE( msg );  
        }  
        sprintf( msg,  "scale:  %f\n", drawing_info.drawing_scale );  
        WRITE( msg );  
        if(drawing_info.projection_angle == UF_DRAW_THIRD_ANGLE_PROJECTION )  
        {  
            sprintf( msg, "projection angle : third \n" );  
            WRITE( msg );  
        }  
        else  
        {  
            sprintf( msg, "projection angle : first \n" );  
            WRITE( msg );  
            sprintf( msg, "========================================\n");  
            WRITE( msg );  
        }  
        switch (drawing_info.units)  
        {  
            case UF_PART_METRIC:  
                sprintf( msg, "Drawing Units: METRIC\n" );  
                break;     
            case UF_PART_ENGLISH:  
                sprintf( msg, "Drawing Units: ENGLISH\n" );  
                break;     
        }  
        WRITE( msg );  
    }  
    static void do_it(void)  
    {  
        int  
            part_units = 0;  
        tag_t  
            part = NULL_TAG,  
            drawing = NULL_TAG;  
        char  
            fspec[MAX_FSPEC_SIZE + 1],  
            msg[300] = { "" };  
        part = UF_PART_ask_display_part();  
        UF_UI_exit_listing_window();  
        UF_PART_ask_part_name(part, fspec);  
            sprintf( msg, "__________________________________________\n");  
            WRITE( msg );  
            sprintf( msg, "Drawings found in %s:\n", fspec);  
            WRITE( msg );  
            sprintf( msg, "__________________________________________\n");  
            WRITE( msg );  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
        report_drawing_info(drawing);  
        }  
        UF_PART_ask_units( UF_PART_ask_display_part(), &part_units );  
        switch ( part_units )  
        {  
            case UF_PART_METRIC:  
                sprintf( msg, "\nPart Units: METRIC\n" );  
                break;     
            case UF_PART_ENGLISH:  
                sprintf( msg, "\nPart Units: ENGLISH\n" );  
                break;     
        }  
        WRITE( msg );  
        sprintf( msg, "              -finished-             \n");  
        WRITE( msg );  
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

> 这段NX二次开发代码的主要功能是遍历当前显示零件中的所有图纸，并报告每个图纸的信息。具体来说：
>
> 1. 定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误信息。
> 2. 定义了ask_next_drawing函数，用于遍历零件中的所有图纸，返回下一个图纸的tag。
> 3. 定义了report_drawing_info函数，用于报告一个图纸的信息，包括图纸名称、尺寸、比例尺、投影角度等。
> 4. 在do_it函数中，首先获取当前显示的零件，然后遍历该零件中的所有图纸，对每个图纸调用report_drawing_info函数报告信息。
> 5. 获取并报告当前零件的单位制信息。
> 6. ufusr函数是NX调用的主函数，其中初始化NX环境，调用do_it函数执行主要逻辑，然后终止NX环境。
> 7. ufusr_ask_unload函数用于卸载用户函数。
>
> 总体来说，这段代码实现了遍历和报告零件图纸信息的功能，展现了典型的NX二次开发流程。
>
