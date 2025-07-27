### 【0372】ask view lower left corner 获取视图左下角坐标

#### 代码

```cpp
    static void ask_view_lower_left_corner(double lower_left[3])  
    {  
    /* This function demonstrates how to determine the current view's lower  
       left corner taking into account any zooming or panning the user may  
       have done.  A GRIP program is used to create a point at the view's  
       lower left corner.  
       This is the GRIP code used:  
            entity/ pt, tpt(2), tln(2)  
            ufargs/ pt  
            tpt(1) = point/0,0,0  
            tln(1) = line/ tpt(1), atangl, 90  
            tpt(2) = point/ ENDOF, YSMALL, tln(1)  
            tln(2) = line/ tpt(2), atangl, 0  
            pt = point/ ENDOF, xsmall, tln(2)  
            delete/ tpt,tln  
            halt  
    */  
        tag_t  
            point;  
        UF_args_t  
            arg;  
        UF_UNDO_mark_id_t  
            mark;  
        arg.type    = UF_TYPE_TAG_T;  
        arg.length  = 1;  
        arg.address = &point;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        set_wcs_to_view("");  
        UF_CALL(UF_call_grip("grip/get_view_lower_left.grx", 1, &arg));  
        UF_CALL(UF_CURVE_ask_point_data(point, lower_left));  
    /*  The UNDO puts the WCS back where it was and deletes the GRIP point 译:The UNDO command will return the WCS to its original position and delete the GRIP point. */  
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
    }

```

#### 代码解析

> 这段代码是用于在NX中获取当前视图的左下角坐标。其主要功能包括：
>
> 1. 使用GRIP程序创建一个点，该点位于当前视图的左下角。
> 2. 将工作坐标系WCS设置为视图坐标系，以确保获取的坐标是视图相关的。
> 3. 调用GRIP程序，传入一个参数，用于获取左下角坐标。
> 4. 读取该点的坐标数据，并将其存储在传入的参数数组中。
> 5. 恢复工作坐标系WCS到原来的状态。
> 6. 删除GRIP程序中创建的点。
> 7. 提供一个函数接口，用于获取当前视图的左下角坐标。
>
> 这段代码通过GRIP程序获取视图相关的坐标，考虑了用户的缩放和平移操作，同时使用UNDO管理，确保了操作的可逆性。
>
