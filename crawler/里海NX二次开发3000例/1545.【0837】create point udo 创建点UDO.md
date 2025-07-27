### 【0837】create point udo 创建点UDO

#### 代码

```cpp
    /*HEAD CREATE_POINT_UDO CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_udo.hxx>  
    #include <ug_string.hxx>  
    #include <ug_info_window.hxx>  
    #if ! defined ( __hp9000s800 ) && ! defined ( __sgi ) && ! defined ( __sun )  
    #    include <strstream>  
    #    include <iostream>  
          using std::ostrstream;  
          using std::endl;   
          using std::ends;  
          using std::cerr;  
    #else  
    #    include <strstream.h>  
    #    include <iostream.h>  
    #endif  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog 是 V18 版本新增的功能。因此，我理解文档的翻译为：UF_print_syslog 是在 V18 版本中新增加的功能。 */  
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
    static UF_UDOBJ_class_t PT_class_id;  
    DllExport extern UF_UDOBJ_class_t get_PT_class_id(void)  
    {  
        return PT_class_id;  
    }  
    DllExport extern void map_abs2wcs(std::vector <double> &cc)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12],  
            temp[3];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs, wcs, mx, &irc);  
        for (int ii = 0; ii < 3; ii++) temp[ii] = cc[ii];  
        FTN(uf5941)(temp, mx);  
        for (ii = 0; ii < 3; ii++) cc[ii] = temp[ii];  
    }  
    DllExport extern void info_pt_cb(tag_t udo_tag)  
    {  
        char  
            tbuffer[UF_UI_MAX_STRING_LEN + 1];  
        try  
        {  
            UgSession my_session(true);  
            UgInfoWindow info;  
            info.open();  
            UgUDO *myudo = dynamic_cast < UgUDO * > ( UgObject::find (udo_tag));  
            std::vector <double> abs_loc = myudo->getDoubles();  
            std::vector <double> wcs_loc = myudo->getDoubles();  
            map_abs2wcs(wcs_loc);  
            info.write(myudo->getName());  
            info.write("\n");  
            sprintf(tbuffer, "UDO coordinates\t\tXC = %14.8f\tX = %14.8f\n",  
                abs_loc[0], wcs_loc[0]);  
            info.write(tbuffer);  
            sprintf(tbuffer, "\t\t\tYC = %14.8f\tY = %14.8f\n",  
                abs_loc[1], wcs_loc[1]);  
            info.write(tbuffer);  
            sprintf(tbuffer, "\t\t\tZC = %14.8f\tZ = %14.8f\n",  
                abs_loc[2], wcs_loc[2]);  
            info.write(tbuffer);  
        }  
        catch (UgException oops)  
        {  
            cout << oops.askErrorText().c_str() << endl;  
            return;  
        }  
    }  
    DllExport extern void display_pt_cb(tag_t udo_tag, void *display_context)  
    {  
        try  
        {  
            UF_DISP_inquire_t display_info;  
            double loc[3];  
            UgSession my_session(true);  
            UF_CALL(UF_DISP_ask_display_context(display_context, &display_info));  
            if (display_info.is_draw_open_disp == TRUE) return;  
            UgUDO *myudo = dynamic_cast < UgUDO * > ( UgObject::find (udo_tag));  
            std::vector <double> abs_loc = myudo->getDoubles();  
            if (abs_loc.size() == 3)  
            {  
                for (int ii = 0; ii < 3; ii++) loc[ii] = abs_loc[ii];  
                UF_CALL(UF_DISP_display_points(loc, 1, UF_DISP_SQUARE,  
                    display_context));  
                UF_CALL(UF_DISP_display_text( (char *) myudo->getName().c_str(),  
                    loc, UF_DISP_MIDDLECENTER, display_context));  
            }  
        }  
        catch (UgException oops)  
        {  
            cout << oops.askErrorText().c_str() << endl;  
            return;  
        }  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        try  
        {  
            UgSession my_session(true);  
            UF_CALL(UF_UDOBJ_create_class("Point_UDO", "Point UDO", &PT_class_id));  
            UF_CALL(UF_UI_add_to_class_sel(PT_class_id));  
            UF_CALL(UF_UDOBJ_register_attn_pt_cb(PT_class_id, display_pt_cb));  
            UF_CALL(UF_UDOBJ_register_display_cb(PT_class_id, display_pt_cb));  
            UF_CALL(UF_UDOBJ_register_fit_cb(PT_class_id, display_pt_cb));  
            UF_CALL(UF_UDOBJ_register_info_obj_cb(PT_class_id, info_pt_cb));  
            UF_CALL(UF_UDOBJ_register_select_cb(PT_class_id, display_pt_cb));  
        }  
        catch (UgException oops)  
        {  
            cout << oops.askErrorText().c_str() << endl;  
            return;  
        }  
    }  
    static logical specify_point(char *prompt, std::vector < double > & location)  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        double  
            loc[3];  
        irc = uc1616(prompt, ia2, ip3, loc);  
        for (int ii = 0; ii < 3; ii++) location[ii] = loc[ii];  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            pt_udo;  
        char  
            messg[UF_UI_MAX_STRING_LEN + 1],  
            name[MAX_ENTITY_NAME_SIZE+1];  
        std::vector  
            < double > loc(3);  
        for (UgIterator<UgUDO> it; !it.isFinished(); it.findNext())  
        {  
            if ((*it)->askClassId() == get_PT_class_id())  
            {  
                info_pt_cb((*it)->getTag());  
                ii++;  
            }  
        }  
        while (sprintf(messg, "Indicate UDO %d position", ++ii) &&  
               specify_point(messg, loc))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_PT_class_id(), &pt_udo));  
            UgUDO *myudo = dynamic_cast < UgUDO * > ( UgObject::find (pt_udo));  
            myudo->addDoubles(loc);  
            sprintf(name, "Point_UDO_%d", ii);  
            myudo->setName(std::string (name));  
            UF_CALL(UF_DISP_add_item_to_display(pt_udo));  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        try  
        {  
            UgSession my_session(true);  
            do_it();  
        }  
        catch (UgException oops)  
        {  
            cout << oops.askErrorText().c_str() << endl;  
            return;  
        }  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现了一个自定义的点UDO(User Defined Object)类，并实现了相关的创建、显示和查询功能。
>
> 代码的主要功能包括：
>
> 1. 创建UDO类：在ufsta函数中，使用UF_UDOBJ_create_class函数创建了一个名为"Point_UDO"的UDO类，并设置了类ID为PT_class_id。
> 2. 注册回调函数：在ufsta函数中，为该UDO类注册了显示、拟合、信息、选择等回调函数，这些函数会在对应的操作时被调用。
> 3. 显示回调函数：display_pt_cb函数实现了点的显示功能，根据UDO的位置信息在图形区绘制点并显示名称。
> 4. 信息回调函数：info_pt_cb函数实现了查询UDO信息的功能，可以显示UDO的名称、绝对坐标和世界坐标。
> 5. 创建UDO对象：do_it函数中，通过循环调用specify_point函数，提示用户指定UDO的位置，并创建相应的UDO对象，添加到显示列表。
> 6. 坐标转换：map_abs2wcs函数实现了从绝对坐标系到世界坐标系的转换。
> 7. 错误处理：使用UF_CALL宏和report_error函数实现了对NX API调用错误的捕获和打印。
> 8. NX会话管理：使用UgSession类管理NX会话。
>
> 总体来说，这段代码通过UDO技术实现了自定义点的创建和显示，并提供了查询功能，是一个典型的NX二次开发示例。
>
