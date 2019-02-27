package qiugong.com.myapplication;

import android.content.Context;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import qiugong.com.myapplication.objects.ParticleShooter;
import qiugong.com.myapplication.objects.ParticleSystem;
import qiugong.com.myapplication.objects.SkyBox;
import qiugong.com.myapplication.programs.ParticleShaderProgram;
import qiugong.com.myapplication.programs.SkyBoxShaderProgram;
import qiugong.com.myapplication.util.Geometry;
import qiugong.com.myapplication.util.MatrixHelper;
import qiugong.com.myapplication.util.TextureHelper;

/**
 * @author qzx 2018/11/2.
 */
class ParticlesRenderer implements GLSurfaceView.Renderer {

    private final Context context;

    private final float[] projectionMatrix = new float[16];
    private final float[] viewMatrix = new float[16];
    private final float[] viewProjectionMatrix = new float[16];

    private ParticleShaderProgram particleProgram;
    private ParticleSystem particleSystem;
    private ParticleShooter redParticleShooter;
    private ParticleShooter greenParticleShooter;
    private ParticleShooter blueParticleShooter;

    private SkyBoxShaderProgram skyBoxProgram;
    private SkyBox skybox;

    private long globalStartTime;
    private int particleTexture;
    private int skyBoxTexture;

    private float xRotation, yRotation;

    ParticlesRenderer(Context context) {
        this.context = context;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        skyBoxProgram = new SkyBoxShaderProgram(context);
        skybox = new SkyBox();

        particleProgram = new ParticleShaderProgram(context);
        particleSystem = new ParticleSystem(10_000);
        globalStartTime = System.nanoTime();

        final Geometry.Vector particleDirection = new Geometry.Vector(0f, 0.5f, 0f);
        final float angleVarianceInDegrees = 5f;
        final float speedVariance = 1f;

        redParticleShooter = new ParticleShooter(
                new Geometry.Point(-1f, 0f, 0f),
                particleDirection,
                Color.rgb(255, 50, 5),
                angleVarianceInDegrees,
                speedVariance);

        greenParticleShooter = new ParticleShooter(
                new Geometry.Point(0f, 0f, 0f),
                particleDirection,
                Color.rgb(25, 255, 25),
                angleVarianceInDegrees,
                speedVariance);

        blueParticleShooter = new ParticleShooter(
                new Geometry.Point(1f, 0f, 0f),
                particleDirection,
                Color.rgb(5, 50, 255),
                angleVarianceInDegrees,
                speedVariance);

        particleTexture = TextureHelper.loadTexture(context, R.drawable.particle_texture);
        skyBoxTexture = TextureHelper.loadCubeMap(context, new int[]{
                R.drawable.left, R.drawable.right,
                R.drawable.bottom, R.drawable.top,
                R.drawable.front, R.drawable.back});
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);

        MatrixHelper.perspectiveM(projectionMatrix, 45, (float) width / (float) height, 1f, 10f);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        drawSkyBox();
        drawParticles();
    }

    private void drawSkyBox() {
        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.rotateM(viewMatrix, 0, -yRotation, 1f, 0f, 0f);
        Matrix.rotateM(viewMatrix, 0, -xRotation, 0f, 1f, 0f);
        Matrix.multiplyMM(viewProjectionMatrix, 0, projectionMatrix, 0, viewMatrix, 0);
        skyBoxProgram.useProgram();
        skyBoxProgram.setUniforms(viewProjectionMatrix, skyBoxTexture);
        skybox.bindData(skyBoxProgram);
        skybox.draw();
    }

    private void drawParticles() {
        float currentTime = (System.nanoTime() - globalStartTime) / 1_000_000_000f;

        redParticleShooter.addParticles(particleSystem, currentTime, 1);
        greenParticleShooter.addParticles(particleSystem, currentTime, 1);
        blueParticleShooter.addParticles(particleSystem, currentTime, 1);

        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.rotateM(viewMatrix, 0, -yRotation, 1f, 0f, 0f);
        Matrix.rotateM(viewMatrix, 0, -xRotation, 0f, 1f, 0f);
        Matrix.translateM(viewMatrix, 0, 0f, -1.5f, -5f);
        Matrix.multiplyMM(viewProjectionMatrix, 0, projectionMatrix, 0, viewMatrix, 0);

        GLES20.glEnable(GLES20.GL_BLEND);
        GLES20.glBlendFunc(GLES20.GL_ONE, GLES20.GL_ONE);

        particleProgram.useProgram();
        particleProgram.setUniforms(viewProjectionMatrix, currentTime, particleTexture);
        particleSystem.bindData(particleProgram);
        particleSystem.draw();

        GLES20.glDisable(GLES20.GL_BLEND);
    }

    void handleTouchDrag(float deltaX, float deltaY) {
        xRotation += deltaX / 16f;
        yRotation += deltaY / 16f;

        if (yRotation < -90) {
            yRotation = -90;
        } else if (yRotation > 90) {
            yRotation = 90;
        }
    }
}
