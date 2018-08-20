//---------------------------------//
//  This file is part of MuJoCo    //
//  Written by Emo Todorov         //
//  Copyright (C) 2017 Roboti LLC  //
//---------------------------------//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include <iostream>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>


#include "mujoco.h"
#include "glfw3.h"

// MuJoCo data structures
mjModel *m = NULL; // MuJoCo model
mjData *d = NULL;  // MuJoCo data
mjvCamera cam;     // abstract camera
mjvOption opt;     // visualization options
mjvScene scn;      // abstract scene
mjrContext con;    // custom GPU context

// mouse interaction
bool button_left = false;
bool button_middle = false;
bool button_right = false;
double lastx = 0;
double lasty = 0;

// keyboard callback
void keyboard(GLFWwindow *window, int key, int scancode, int act, int mods)
{
    // backspace: reset simulation
    if (act == GLFW_PRESS && key == GLFW_KEY_BACKSPACE)
    {
        mj_resetData(m, d);
        mj_forward(m, d);
    }
}

// mouse button callback
void mouse_button(GLFWwindow *window, int button, int act, int mods)
{
    // update button state
    button_left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    button_middle = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    button_right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

    // update mouse position
    glfwGetCursorPos(window, &lastx, &lasty);
}

// mouse move callback
void mouse_move(GLFWwindow *window, double xpos, double ypos)
{
    // no buttons down: nothing to do
    if (!button_left && !button_middle && !button_right)
        return;

    // compute mouse displacement, save
    double dx = xpos - lastx;
    double dy = ypos - lasty;
    lastx = xpos;
    lasty = ypos;

    // get current window size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // get shift key state
    bool mod_shift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                      glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

    // determine action based on mouse button
    mjtMouse action;
    if (button_right)
        action = mod_shift ? mjMOUSE_MOVE_H : mjMOUSE_MOVE_V;
    else if (button_left)
        action = mod_shift ? mjMOUSE_ROTATE_H : mjMOUSE_ROTATE_V;
    else
        action = mjMOUSE_ZOOM;

    // move camera
    mjv_moveCamera(m, action, dx / height, dy / height, &scn, &cam);
}

// scroll callback
void scroll(GLFWwindow *window, double xoffset, double yoffset)
{
    // emulate vertical mouse motion = 5% of window height
    mjv_moveCamera(m, mjMOUSE_ZOOM, 0, -0.05 * yoffset, &scn, &cam);
}




/////TRIAL: test wall contact
void testWallContact(const mjModel* m, mjData* d)
{
    
if (d -> time < 0.1)
		{  
			  d -> ctrl[0] = {0.04};;
		}
if (d -> time > 0.1 && d -> time < 1)
		{  
			  d -> ctrl[0] = {0};;
		}
      std::cout << "nconmax:" << m->nconmax << std::endl; 
      std::cout << "contactpos[0]:" << d->contact->pos[0] << std::endl; 
}




//main functon
int main(int argc, const char **argv)
{
    // check command-line arguments
    if (argc != 2)
    {
        printf(" USAGE:  basic modelfile\n");
        return 0;
    }

    // activate software
    mj_activate("mjkey.txt");
    // load and compile model
    char error[1000] = "Could not load binary model";
    if (strlen(argv[1]) > 4 && !strcmp(argv[1] + strlen(argv[1]) - 4, ".mjb"))
       { m = mj_loadModel(argv[1], 0);
        std::cout<<argv[1][0]<<std::endl;}
    else
        m = mj_loadXML(argv[1], 0, error, 1000);
    if (!m)
        mju_error_s("Load model error: %s", error);

    // make data
    d = mj_makeData(m);

    // init GLFW
    if (!glfwInit())
        mju_error("Could not initialize GLFW");

    // create window, make OpenGL context current, request v-sync
    GLFWwindow *window = glfwCreateWindow(1200, 900, "Demo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // initialize visualization data structures
    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjr_defaultContext(&con);
    mjv_makeScene(&scn, 1000);                 // space for 1000 objects
    mjr_makeContext(m, &con, mjFONTSCALE_100); // model-specific context

    // install GLFW mouse and keyboard callbacks
    glfwSetKeyCallback(window, keyboard);
    glfwSetCursorPosCallback(window, mouse_move);
    glfwSetMouseButtonCallback(window, mouse_button);
    glfwSetScrollCallback(window, scroll);

    // print some arguments
    // m -> nu = 2;    
    // run main loop, target real-time simulation and 60 fps rendering
    while (!glfwWindowShouldClose(window))
    {
        // advance interactive simulation for 1/60 sec
        //  Assuming MuJoCo can simulate faster than real-time, which it usually can,
        //  this loop will finish on time for the next frame to be rendered at 60 fps.
        //  Otherwise add a cpu timer and exit this loop when it is time to render.
        mjtNum simstart = d->time;
/*         while ((d->time - simstart < 1.0 / 60.0) && (d->time < 11))
        {
            mj_step1(m, d);
            applyForceOnModel(m, d);
            std::cout << m->nu << "  " << d->ctrl[0] << std::endl;
            std::cout << d->time << std::endl;
            mj_step2(m, d);
        } */


/////output Position Velocity Control To Files
//std::vector<double> position(d->qpos,d->qpos+2);
//output data to position.txt; 
const char* positionFilename = "position.txt";
std::ofstream positionFile(positionFilename, std::ios::app);
const char* velocityFilename = "velocity.txt";
std::ofstream velocityFile(velocityFilename, std::ios::app);
const char* accelerationFilename = "acceleration.txt";
std::ofstream accelerationFile(accelerationFilename, std::ios::app);
const char* contactFilename = "contact.txt";
std::ofstream contactFile(contactFilename, std::ios::app);
//positionFile.open(positionFilename, std::ios::out | std::ios::trunc);

         while ((d->time - simstart < 1.0 / 200.0) && (d->time < 1))
        {
  
            mj_step1(m, d);
            std::cout << "-------------------------------------" << std::endl; 
  	    std::cout << "simulation time: " << d->time << std::endl; 
   	    std::cout << "number of generalized coordinates = dim(qpos)" << m->nq << std::endl; 
   	    std::cout << "number of degrees of freedom = dim(qvel):" << m->nv << std::endl; 
   	    std::cout << "number of actuators/controls = dim(ctrl)" << m->nu << std::endl; 
   	    std::cout << "number of activation states = dim(act)" << m->na << std::endl; 
	    std::cout << "qpos: " << d->qpos[0] <<  " " << d->qpos[1] <<" " << d->qpos[2] <<" " << d->qpos[3] << " " << d->qpos[4] << std::endl; 
	    std::cout << "qvel: " << d->qvel[0] <<  " " << d->qvel[1] <<" " << d->qvel[2] <<" " << d->qvel[3] << " " << d->qvel[4] << std::endl; 
	    std::cout << "qacc: " << d->qacc[0] <<  " " << d->qacc[1] <<" " << d->qacc[2] <<" " << d->qacc[3] << " " << d->qacc[4] << std::endl; 
   	    std::cout << "d->act: " << d->act[0] <<  " " << d->act[1] << std::endl;
            std::cout << "d->ctrl:" << d->ctrl[0] << " " << d->ctrl[1] <<std::endl;
            std::cout << "d->qfrc_applied:" << d->qfrc_applied[0] << std::endl;
	    std::cout << "efc_force[0](njmax x 1):" << d->efc_force[0]<< " " << d->efc_force[1]<< " " << d->efc_force[2]<< " " << d->efc_force[3]<< std::endl;
	    std::cout << "qfrc_constraint(nv x 1):" << d->qfrc_constraint[0]<< " " << d->qfrc_constraint[1]<< " " << d->qfrc_constraint[2]<< " " << d->qfrc_constraint[3]<< " " << d->qfrc_constraint[4]<< " " <<  d->qfrc_constraint[5]<< " " << d->qfrc_constraint[6]<< std::endl;
	    std::cout << "actuator_force(nu x 1):" << d->actuator_force[0]<< " " << d->actuator_force[1]<< " " << d->actuator_force[2]<< " " << d->actuator_force[3]<< " " << d->actuator_force[4]<< " " << d->actuator_force[5]<< " " << d->actuator_force[6]<< " " << d->actuator_force[7]<< " " << d->actuator_force[8]<<std::endl;
	    std::cout << "qfrc_actuator(nv x 1):" << d->qfrc_actuator[0]<< " " << d->qfrc_actuator[1]<< " " << d->qfrc_actuator[2]<< " " << d->qfrc_actuator[3]<< " " << d->qfrc_actuator[4]<< " " << d->qfrc_actuator[5]<< " " << d->qfrc_actuator[6]<< " " << d->qfrc_actuator[7]<< " " << d->qfrc_actuator[8]<< std::endl;		
	  std::cout << "number of equality constraints: " << d->ne << std::endl; 
	  std::cout << "number of friction constraints: " << d->nf << std::endl; 
	  std::cout << "number of constraints: " << d->nefc << std::endl; 
	  std::cout << "number of detected contacts: " << d->ncon << std::endl; 
    



		//if (d -> time < 0.2){  
		//	testForceOnAccCorrect(m, d);
		//}
  		//if (d -> time > 0.2 && d -> time < 1){  
			testWallContact(m, d);
		//}

//derive contact force
 	mjtNum result[6];

	 if (d->ncon == 0){	  
             		contactFile << 0 << " " << 0 <<" " << 0 <<" " << 0 <<" " << 0 <<" " << 0<< std::endl;
			}

        for (int i = 0; i < d->ncon; i++) {
	  mj_contactForce(m,d,i,result);
          std::cout << "contactForce result: "  << "id: "   << i << " " << "result: " << result[0] << " " << result[1] <<" " << result[2] <<" " << result[3] <<" " << result[4] <<" " << result[5] <<std::endl; 
			contactFile << result[0] << " " << result[1] <<" " << result[2] <<" " << result[3] <<" " << result[4] <<" " << result[5]<< std::endl;
			
	}
          std::cout << "-------------------------------------" << std::endl; 
 
           for (int i = 0; i < m->nq; i++) {
		positionFile << d->qpos[i] << " ";
		}
		positionFile << std::endl;
           for (int i = 0; i < m->nv; i++) {
		velocityFile << d->qvel[i] << " ";
		}
		velocityFile << std::endl;
	   for (int i = 0; i < m->nv; i++) {
		accelerationFile << d->qacc[i] << " ";
		}
		accelerationFile << std::endl;

            mj_step2(m, d);

        } 
                positionFile.close(); 
                velocityFile.close();  
                accelerationFile.close();   
		contactFile.close();   



        // get framebuffer viewport
        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

        // update scene and render
        mjv_updateScene(m, d, &opt, NULL, &cam, mjCAT_ALL, &scn);
        mjr_render(viewport, &scn, &con);

        // swap OpenGL buffers (blocking call due to v-sync)
        glfwSwapBuffers(window);

        // process pending GUI events, call GLFW callbacks
        glfwPollEvents();
    }

    // close GLFW, free visualization storage
    glfwTerminate();
    mjv_freeScene(&scn);
    mjr_freeContext(&con);

    // free MuJoCo model and data, deactivate
    mj_deleteData(d);
    mj_deleteModel(m);
    mj_deactivate();

    return 1;
}
