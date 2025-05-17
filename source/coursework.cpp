#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>



// Function prototypes
void keyboardInput(GLFWwindow *window);
void mouseInput(GLFWwindow* window);


Camera camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -2.0f)); //creates the camera object

struct Object {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};

struct Light {
    glm::vec3 position;
    glm::vec3 colour;
    float constant;
    float linear;
    float quadratic;
    unsigned int type;
};

std::vector<Light> lightSources;




float previousTime = 0.0f;
float timeChange = 0.0f;




int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
	window = glfwCreateWindow(1920, 1080, "Computer Graphics Coursework", NULL, NULL); //just changed the aspect ratio for a 1080p window
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    
    glEnable(GL_DEPTH_TEST); //This enables dpeth test. Only showing the pixels that are closest to the camera
    //glEnable(GL_CULL_FACE); //so that only the front facing sides are rendered in
	Model Sphere("../assets/sphere.obj"); //loads the sphere model
    Model donkeyKong("../assets/Donkey_Kong.obj"); 
    donkeyKong.addTexture("../assets/blue.bmp", "diffuse");
	Sphere.addTexture("../assets/neutral_specular.png", "diffuse");


    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //setting and capturing the mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //capture mouse input
    glfwPollEvents(); //checks for a mouse input 
    glfwSetCursorPos(window, 1920 / 2, 1080 / 2); //sets the mouse cursor to the center of our window

    const float verticies[]{
        //x     y      z
        //front verticies 
        -1.0f, -1.0f, 1.0f, // 0 - bottom left
        1.0f, -1.0f, 1.0f, // 1 - bottom right
		1.0f, 1.0f, 1.0f, // 2 - top right
        -1.0f, - 1.0f, 1.0f, // 3 - top left    
        1.0f, 1.0f, 1.0f, 
        -1.0f, 1.0f, 1.0f,
		//right verticies
        1.0f, -1.0f, 1.0f, 
        1.0f, -1.0f, -1.0f, 
        1.0f, 1.0f, -1.0f, 
        1.0f, -1.0f, 1.0f, 
        1.0f, 1.0f, -1.0f, 
        1.0f, 1.0f, 1.0f,
		//back verticies
        1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f, 1.0f, -1.0f, 
        1.0f, -1.0f, -1.0f, 
        -1.0f, 1.0f, -1.0f, 
        1.0f, 1.0f, -1.0f,
		//left verticies
        -1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f, 1.0f, 
        -1.0f, 1.0f, 1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f, 1.0f, 1.0f, 
        -1.0f, 1.0f, -1.0f, 
		//bottom verticies
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 
        1.0f, -1.0f, 1.0f, 
        -1.0f, -1.0f, -1.0f, 
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        //top verticies
        -1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, -1.0f, 
        -1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, -1.0f, 
        -1.0f, 1.0f, -1.0f, 
    };

    const float uv[]{
		// front uv verticies
        0.0f, 0.0f, //0
		1.0f, 0.0f, //1
		1.0f, 1.0f, //2
		0.0f, 0.0f, //3
		1.0f, 1.0f, //4
		0.0f, 1.0f, //5
		//right uv verticies
		0.0f, 0.0f, //6
		1.0f, 0.0f, //7
		1.0f, 1.0f, //8
		0.0f, 0.0f, //9
		1.0f, 1.0f, //10
		0.0f, 1.0f, //11
		//back uv verticies
		0.0f, 0.0f, //12
		1.0f, 0.0f, //13
		1.0f, 1.0f, //14
		0.0f, 0.0f, //15
		1.0f, 1.0f, //16
		0.0f, 1.0f, //17
		//left uv verticies
		0.0f, 0.0f, //18
		1.0f, 0.0f, //19
		1.0f, 1.0f, //20
		0.0f, 0.0f, //21
		1.0f, 1.0f, //22
		0.0f, 1.0f, //23
		//bottom uv verticies
		0.0f, 0.0f, //24
		1.0f, 0.0f, //25
		1.0f, 1.0f, //26
		0.0f, 0.0f, //27
		1.0f, 1.0f, //28
		0.0f, 1.0f, //29
		//top uv verticies
		0.0f, 0.0f, //30    
		1.0f, 0.0f, //31
		1.0f, 1.0f, //32
		0.0f, 0.0f, //33
		1.0f, 1.0f, //34
		0.0f, 1.0f, //35

    };

    static const unsigned int indices[]{
        0,   1,  2,  3,  4,  5,     
        6,   7,  8,  9, 10, 11,     
        12, 13, 14, 15, 16, 17,     
        18, 19, 20, 21, 22, 23,     
        24, 25, 26, 27, 28, 29,     
        30, 31, 32, 33, 34, 35      
    };



	unsigned int VAO; // Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	unsigned int VBO; // Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	unsigned int EBO; // Element Buffer Object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    unsigned int uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);


    unsigned int shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl"); //compiles the shader program

	unsigned int texture; // Texture ID
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    const char* path = "../assets/cat.jpg";
    int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data)
        std::cout << "Texture Loaded Successfully" << std::endl;
	else
		std::cout << "Failed to load texture" << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

    unsigned int roomTexture;
    glGenTextures(1, &roomTexture);
	glBindTexture(GL_TEXTURE_2D, roomTexture);
	const char* roomPath = "../assets/bricks.jpg";
    int roomWidth, roomHeight, roomNChannels;
	stbi_set_flip_vertically_on_load(true);
    unsigned char* roomData = stbi_load(roomPath, &roomWidth, &roomHeight, &roomNChannels, 0);
    if (roomData)
        std::cout << "Room Brick Texture Loaded Successfullu" << std::endl;
    else
		std::cout << "Failed to Load Room Brick Texture" << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, roomWidth, roomHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, roomData);
	glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(roomData);




	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //improved texture filtering. Linear texture filtering and linear interpolation of mipmaps

    //the cubes positions
    glm::vec3 positions[] = {
    glm::vec3(1.0f,  3.0f,  2.0f), //first cube
    glm::vec3(2.0f,  -5.0f, 10.0f), //second cube 
    glm::vec3(-3.0f, 2.0f, -3.0f), //third cube 
    glm::vec3(-4.0f, 2.0f, -8.0f), //forth cube 
    glm::vec3(2.0f,  -2.0f, 6.0f), //fifth cube
	glm::vec3(6.0f,  5.0f, -10.0f), //sixth cube
    };


    //adding the cubes to the objects vector

    std::vector<Object> objects;
    Object object;
    object.name = "Cubes";
    for (unsigned int i = 0; i < 6; i++)
    {
        object.position = positions[i];
        object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        object.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        object.angle = Maths::radians(20.0f * i);
		objects.push_back(object); //adds the object to the vector
    }

    Object dkObject;
    dkObject.name = "DonkeyKong";
    dkObject.position = glm::vec3(-1.0f, -1.0f, -1.0f); 
    dkObject.scale = glm::vec3(1.0f); 
    dkObject.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    dkObject.angle = 0.0f;
    objects.push_back(dkObject);

    Object room;
	room.name = "Room";
	room.position = glm::vec3(0.0f, 0.0f, 0.0f); 
	room.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	room.scale = glm::vec3(10.0f, 10.0f, 10.0f); 
	room.angle = 0.0f;
	objects.push_back(room);    



    camera.eye = glm::vec3(0.0f, 0.0f, 4.0f);
    camera.target = objects[0].position;

	donkeyKong.ka = 0.7f; //ambient light intensity
	donkeyKong.kd = 0.5f; //diffuse light intensity
	donkeyKong.ks = 1.0f; //specular light intensity
	donkeyKong.Ns = 20.0f; //specular exponent
	float constant = 1.0f; 
    float quadratic = 0.02f;
    float linear = 0.1f;

    glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 0.5f);

    Light light;
    light.position = glm::vec3(2.0f, 2.0f, 2.0f);
    light.colour= glm::vec3(1.0f, 1.0f, 1.0f);
	light.constant = 1.0f;
	light.linear = 0.1f;
	light.quadratic = 0.02f;
	light.type = 1;
	lightSources.push_back(light); //adds the light to the vector

    light.position = glm::vec3(1.0f, 1.0f, -8.0f);
    lightSources.push_back(light);


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        //updating the keyboard timer input
		float time = glfwGetTime();
		timeChange = time - previousTime;
		previousTime = time;

        glfwPollEvents();


        // Get inputs
        mouseInput(window); //for mouse inputsww
        keyboardInput(window);

        // Clear the window
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		glEnableVertexAttribArray(0); //sending vertex buffer data to the shader
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0,         
            3,         
            GL_FLOAT,  
            GL_FALSE,  
            0,         
            (void*)0); 
        glUseProgram(shaderID);
		glUniform1f(glGetUniformLocation(shaderID, "ka"), donkeyKong.ka);
		glUniform1f(glGetUniformLocation(shaderID, "kd"), donkeyKong.kd);
        glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &lightColour[0]);
		glm::vec3 viewSpaceLightPos = glm::vec3(camera.view * glm::vec4(lightPos, 1.0f)); //view space position of the light
		glUniform3fv(glGetUniformLocation(shaderID, "lightPos"), 1, &viewSpaceLightPos[0]);
		glUniform1f(glGetUniformLocation(shaderID, "ks"), donkeyKong.ks);
		glUniform1f(glGetUniformLocation(shaderID, "Ns"), donkeyKong.Ns);
		glUniform1f(glGetUniformLocation(shaderID, "constant"), constant);
		glUniform1f(glGetUniformLocation(shaderID, "linear"), linear);
		glUniform1f(glGetUniformLocation(shaderID, "quadratic"), quadratic);

		glBindTexture(GL_TEXTURE_2D, texture); //binds the texture to the shader)
        glBindVertexArray(VAO);


        // Send multiple light source properties to the shader
        for (unsigned int i = 0; i < static_cast<unsigned int>(lightSources.size()); i++)
        {
            glm::vec3 viewSpaceLightPosition = glm::vec3(camera.view * glm::vec4(lightSources[i].position, 1.0f));
            std::string idx = std::to_string(i);
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].colour").c_str()), 1, &lightSources[i].colour[0]);
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].position").c_str()), 1, &viewSpaceLightPosition[0]);
            glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].constant").c_str()), lightSources[i].constant);
            glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].linear").c_str()), lightSources[i].linear);
            glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].quadratic").c_str()), lightSources[i].quadratic);
            glUniform1i(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].type").c_str()), lightSources[i].type);
        }

        // Send object lighting properties to the fragment shader
        glUniform1f(glGetUniformLocation(shaderID, "ka"), donkeyKong.ka);
        glUniform1f(glGetUniformLocation(shaderID, "kd"), donkeyKong.kd);
        glUniform1f(glGetUniformLocation(shaderID, "ks"), donkeyKong.ks);
        glUniform1f(glGetUniformLocation(shaderID, "Ns"), donkeyKong.Ns);



		glEnableVertexAttribArray(1); //sending uv buffer data to the shader
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        /*--> Composite transformations in OpenGL*/

        for (int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            objects[i].angle += 0.0f;
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);
            glm::mat4 modelMatrix = translate * rotate * scale;
            glm::mat4 MVP = camera.projection * camera.view * modelMatrix;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glm::mat4 MV = camera.view * modelMatrix;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);

            if (objects[i].name == "Room") {
                glUniform1i(glGetUniformLocation(shaderID, "useLighting"), false);
				glBindTexture(GL_TEXTURE_2D, roomTexture); //binds the texture to the shader)
				glBindVertexArray(VAO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
            }
            else {
				glUniform1i(glGetUniformLocation(shaderID, "useLighting"), true);
				glBindTexture(GL_TEXTURE_2D, texture); //binds the texture to the shader)
            }
            if (objects[i].name == "DonkeyKong") {
                donkeyKong.draw(shaderID);
            }
            else {
                glBindVertexArray(VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0); 
            }
        }
        // ---------------------------------------------------------------------
// Draw light sources
// Activate light source shader
        glUseProgram(shaderID);


        for (unsigned int i = 0; i < static_cast<unsigned int>(lightSources.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(lightSources[i].position);
            glm::mat4 scale = Maths::scale(glm::vec3(0.1f));
            glm::mat4 model = translate * scale;

            // Send the MVP and MV matrices to the vertex shader
            glm::mat4 MVP = camera.projection * camera.view * model;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

            // Send model, view, projection matrices and light colour to light shader
            glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &lightSources[i].colour[0]);

            // Draw light source
            Sphere.draw(shaderID);
        }




		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


        // Swap buffers
        glfwSwapBuffers(window);
    }

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow *window)
{
    camera.calculateCameraVectors();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eye += camera.front * 5.0f * timeChange;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= camera.front * 5.0f * timeChange;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= camera.right * 5.0f * timeChange;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += camera.right * 5.0f * timeChange;
    camera.target = camera.eye + camera.front;
    camera.calculateMatrices();
}

void mouseInput(GLFWwindow* window) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
	glfwSetCursorPos(window, 1920 / 2, 1080 / 2); 

    //updating the yaw and pitch angles
    camera.yaw += 0.0005f * float(xPos - 1920 / 2);
    camera.pitch -= 0.0005f * float(1080 / 2 - yPos);
    camera.calculateCameraVectors();
	camera.target = camera.eye + camera.front; //sets the target to the front vector
    
}
