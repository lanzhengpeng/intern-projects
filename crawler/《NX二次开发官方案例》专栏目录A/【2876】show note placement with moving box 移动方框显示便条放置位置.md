### 【2876】show note placement with moving box 移动方框显示便条放置位置

#### 代码

```cpp
    /*HEAD SHOW_NOTE_PLACEMENT_WITH_MOVING_BOX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void motion_cb( double * pos,  
        UF_UI_motion_cb_data_p_t motion_cb_data, void *client_data )  
    {  
        tag_t  
            view_tag;  
        double   
            pos_array[5][3],  
            *size = (double *) client_data,  
            box_x_size,  
            box_y_size;   
        box_x_size = size[0];  
        box_y_size = size[1];             
        view_tag = motion_cb_data->view_tag;  
        pos_array[0][0] = pos[0] - (box_x_size/2.);  
        pos_array[0][1] = pos[1] - (box_y_size/2.);  
        pos_array[1][0] = pos[0] + (box_x_size/2.);  
        pos_array[1][1] = pos[1] - (box_y_size/2.);  
        pos_array[2][0] = pos[0] + (box_x_size/2.);  
        pos_array[2][1] = pos[1] + (box_y_size/2.);  
        pos_array[3][0] = pos[0] - (box_x_size/2.);  
        pos_array[3][1] = pos[1] + (box_y_size/2.);  
        pos_array[4][0] = pos_array[0][0];  
        pos_array[4][1] = pos_array[0][1];  
        UF_CALL(UF_DISP_display_ogp_polyline ( view_tag, pos_array, 5 ));  
    }  
    static void do_it(void)  
    {  
        int  
            element_30 = 0,  
            mpi[100],  
            num_text = 0,  
            response = 0;  
        tag_t  
            the_note = NULL_TAG,  
            view_tag = NULL_TAG;   
        double  
            mpr[70],  
            size[2] = { 0., 0. },  
            origin[3] = { 0., 0., 0. },  
            screen_pos[3] = { 0., 0., 0. };  
        char  
            diameter[27],  
            radius[27],  
            *text1[1] = { "This note is from UF_DRF_create_note" };  
        UF_DRF_draft_aid_text_info_t    
            *text_info;  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        element_30 = mpi[30];  
        mpi[30] = 5;               /* force entity site to mid-center 里海译:强制实体站点居中 */  
                   /* temporarily place the note, so we can measure it 里海译:暂时放置便条，以便我们测量它。 */   
        UF_CALL(UF_DRF_create_note(1, text1, origin, 0, &the_note));  
        UF_CALL(UF_DRF_ask_draft_aid_text_info ( the_note, &num_text,   
                                                           &text_info ));  
             /* measure the note, so we know how big the box should be 里海译:测量音符的长度，以便我们知道盒子应该有多大。 */  
        size[0] = text_info->length;  
        size[1] = text_info->height;  
        UF_OBJ_delete_object( the_note );  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        UF_CALL( UF_UI_specify_screen_position( "Place the Note:",   
                                                 (UF_UI_motion_fn_t) motion_cb,   
                                                 (void *)size, screen_pos,   
                                                 &view_tag, &response ));  
        if( response == UF_UI_PICK_RESPONSE )  
        {  
            UF_CALL(UF_DRF_create_note(1, text1, screen_pos, 0, &the_note));  
        }  
        /* reset initial preferences 里海译:重置初始偏好设置 */  
        mpi[30] = element_30;  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        UF_DRF_free_text( num_text, &text_info );  
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

> 这段代码是NX的二次开发代码，主要用于创建一个注释，并使其在屏幕上移动。具体步骤如下：
>
> 1. 首先定义了一个错误报告函数report_error，用于在发生错误时输出错误信息。
> 2. 定义了一个运动回调函数motion_cb，用于在注释移动时显示一个跟随注释移动的盒子。注释的位置通过pos参数传递，盒子的尺寸通过client_data参数传递。在回调函数中，根据注释的位置和盒子的尺寸计算盒子的顶点，并使用UF_DISP_display_ogp_polyline函数显示盒子。
> 3. do_it函数是主要逻辑。首先获取当前的注释偏好设置，并临时创建一个注释，以便测量注释的尺寸。然后获取注释的尺寸，删除临时注释，并恢复注释偏好设置。
> 4. 使用UF_UI_specify_screen_position函数让用户指定注释的位置，在指定过程中调用motion_cb函数显示跟随注释移动的盒子。
> 5. 用户指定位置后，在指定位置创建注释。
> 6. 最后恢复注释的初始偏好设置，并释放文本信息内存。
> 7. ufusr函数是NX调用的主函数，其中初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数返回立即卸载，表示NX可以在调用ufusr后立即卸载该二次开发代码。
>
> 总体来说，这段代码实现了在NX中创建一个可以指定位置的注释，并在指定过程中显示一个跟随注释移动的盒子，以直观地显示注释的最终位置。
>
