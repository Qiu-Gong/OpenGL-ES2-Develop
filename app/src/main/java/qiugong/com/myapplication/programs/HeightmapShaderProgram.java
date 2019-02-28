package qiugong.com.myapplication.programs;

import android.content.Context;
import android.opengl.GLES20;

import qiugong.com.myapplication.R;

/**
 * @author qzx 2019/2/27.
 */
public class HeightmapShaderProgram extends ShaderProgram{

    private final int uMatrixLocation;
    private final int aPositionLocation;

    public HeightmapShaderProgram(Context context) {
        super(context, R.raw.heightmap_vertex_shader, R.raw.heightmap_fragment_shader);

        uMatrixLocation = GLES20.glGetUniformLocation(program, U_MATRIX);
        aPositionLocation = GLES20.glGetAttribLocation(program, A_POSITION);
    }

    public void setUniforms(float[] matrix) {
        GLES20.glUniformMatrix4fv(uMatrixLocation, 1, false, matrix, 0);
    }

    public int getPositionAttributeLocation() {
        return aPositionLocation;
    }
}
