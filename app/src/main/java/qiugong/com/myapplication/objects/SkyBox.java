package qiugong.com.myapplication.objects;

import android.opengl.GLES20;

import java.nio.ByteBuffer;

import qiugong.com.myapplication.data.VertexArray;
import qiugong.com.myapplication.programs.SkyBoxShaderProgram;

/**
 * @author qzx 2019/2/27.
 */
public class SkyBox {

    private static final int POSITION_COMPONENT_COUNT = 3;
    private final VertexArray vertexArray;
    private final ByteBuffer indexArray;

    public SkyBox() {
        vertexArray = new VertexArray(new float[]{
                -1, 1, 1,     // (0) Top-left near
                1, 1, 1,      // (1) Top-right near
                -1, -1, 1,    // (2) Bottom-left near
                1, -1, 1,     // (3) Bottom-right near
                -1, 1, -1,    // (4) Top-left far
                1, 1, -1,     // (5) Top-right far
                -1, -1, -1,   // (6) Bottom-left far
                1, -1, -1     // (7) Bottom-right far
        });

        indexArray = ByteBuffer.allocateDirect(6 * 6)
                .put(new byte[]{
                        // Front
                        1, 3, 0,
                        0, 3, 2,

                        // Back
                        4, 6, 5,
                        5, 6, 7,

                        // Left
                        0, 2, 4,
                        4, 2, 6,

                        // Right
                        5, 7, 1,
                        1, 7, 3,

                        // Top
                        5, 1, 4,
                        4, 1, 0,

                        // Bottom
                        6, 2, 7,
                        7, 2, 3});
        indexArray.position(0);
    }

    public void bindData(SkyBoxShaderProgram skyBoxProgram) {
        vertexArray.setVertexAttribPointer(0, skyBoxProgram.getPositionAttributeLocation(),
                POSITION_COMPONENT_COUNT, 0);
    }

    public void draw() {
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, 36, GLES20.GL_UNSIGNED_BYTE, indexArray);
    }
}
