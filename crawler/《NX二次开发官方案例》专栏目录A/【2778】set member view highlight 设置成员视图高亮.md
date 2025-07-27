### 【2778】set member view highlight 设置成员视图高亮

#### 代码

```cpp
    static void set_member_view_highlight(tag_t member_view, int status)  
    {  
        double  
            corners[4],  
            end[3],  
            start[3];  
        UF_OBJ_disp_props_t  
            props[2] = {{ 0, 0, 0, 0, 0, 0}, { 0, UF_OBJ_CYAN, 0, 0, 0, 0}};  
        UF_CALL(UF_DRAW_ask_view_borders(member_view, corners));  
        start[0] = corners[0];  
        start[1] = corners[1];  
        start[2] = 0;  
        end[0] = corners[2];  
        end[1] = corners[1];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
        start[0] = corners[2];  
        start[1] = corners[1];  
        start[2] = 0;  
        end[0] = corners[2];  
        end[1] = corners[3];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
        start[0] = corners[2];  
        start[1] = corners[3];  
        start[2] = 0;  
        end[0] = corners[0];  
        end[1] = corners[3];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
        start[0] = corners[0];  
        start[1] = corners[3];  
        start[2] = 0;  
        end[0] = corners[0];  
        end[1] = corners[1];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
    }

```

#### 代码解析

> 这段NX Open C++代码定义了一个静态函数set_member_view_highlight，其功能是在NX部件的成员视图周围绘制临时线条，以高亮显示选定的成员视图。
>
> 主要步骤如下：
>
> 1. 获取成员视图的边界坐标。
> 2. 根据边界坐标绘制四条临时线条，形成一个框，围住成员视图。
> 3. 根据参数status，线条的颜色可以是UF_OBJ_CYAN(青色)或UF_OBJ_BLACK(黑色)。
> 4. 通过UF_DISP_display_temporary_line函数显示线条，线条会在当前工作视图中显示，并且是临时性的。
> 5. 该函数可以用来在NX部件中选择和突出显示特定的成员视图。
> 6. 参数member_view是要突出显示的成员视图的tag。
> 7. 参数status为0时，线条颜色为黑色；为1时，线条颜色为青色。
> 8. 该函数使用了NX Open C++的UF_DRAW和UF_DISP API。
>
> 总之，这段代码实现了在NX部件中高亮显示选定成员视图的功能，通过绘制临时线条围住成员视图，从而帮助用户快速定位和识别。
>
