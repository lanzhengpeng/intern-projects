### 【0882】create simple holes 创建简单孔

#### 代码

```cpp
    static void create_simple_holes(void)  
    {  
        char  
            angle[20]          = { "0" },  
            depth[20]          = { "1" },  
            dia[20]            = { "1" };  
        double  
            loc[3],  
            z_dir[3];  
        tag_t  
            face,  
            hole;  
        while (((face = select_planar_face("Select face for hole",loc,z_dir))  
            != NULL_TAG) && (prompt_hole_parms(angle, depth, dia)))  
        {  
            UF_CALL(UF_MODL_create_simple_hole(loc, z_dir, dia, depth, angle,  
                face, NULL_TAG, &hole));  
        }  
    }

```

#### 代码解析

> 这段代码是用于在NX中创建简单孔的二次开发代码。其主要功能如下：
>
> 1. 选择平面面：首先，代码会提示用户选择一个平面面作为孔的放置面。用户选择后，代码会获取该面的位置loc和法向量z_dir。
> 2. 输入孔参数：代码会提示用户输入孔的参数，包括角度angle、深度depth和直径dia。这些参数分别表示孔的旋转角度、孔的深度和孔的直径。
> 3. 创建孔：当用户选择了一个面并输入了孔参数后，代码会调用UF_MODL_create_simple_hole函数，在选择的面上创建一个简单孔。孔的位置由loc和z_dir确定，其他参数由用户输入确定。
> 4. 循环创建：代码会持续提示用户选择面并输入参数，直到用户选择取消或退出循环。
> 5. 错误处理：代码使用了UF_CALL宏来调用NX API，它会自动处理API调用中的错误，使代码更加健壮。
>
> 综上所述，这段代码实现了在NX中通过交互方式在用户选择的平面上创建简单孔的功能，用户只需选择面并输入孔参数即可，非常方便易用。
>
