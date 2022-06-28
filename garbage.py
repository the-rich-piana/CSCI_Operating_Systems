###########################################################################

# March 2019, Orit Peleg, orit.peleg@colorado.edu

# Code for HW3 CSCI 4314/5314 Dynamic Models in Biology

##########################################################################


import numpy as np
import math
import matplotlib.pyplot as plt
#import random as rd



class flock():
    def flocking_python(self):

        N = 50 #No. of Boids
        frames = 100 #No. of frames
        limit = 100 #Axis Limits
        L  = limit*2
        P = 10 #Spread of initial position (gaussian)
        V = 10 #Spread of initial velocity (gaussian)
        delta = 1 #Time Step
        c1 = 0.00001 #Attraction Scaling factor
        c2 = 0.1 #Repulsion scaling factor
        c3 = 1 #Heading scaling factor
        c4 = 0.01 #Randomness scaling factor
        vlimit = 1 #Maximum velocity

        #Initialize
        p = P*np.random.randn(2,N)
        v = V*np.random.randn(2,N)
        
        #print(p)

        #Initializing plot
        plt.ion()
        fig = plt.figure(figsize=(10,5))
        ax = fig.add_subplot(111)

        for i in range(frames):
            v1 = np.zeros((2,N))
            v2 = np.zeros((2,N))                 
            v3 = ( (np.sum(v[0,:])/N ) + (np.sum(v[1,:])/N) ) * c3;  #align average heading
            #Calculate Average Velocity v3 
            v4 = np.zeros((2,N))
            
            if (np.linalg.norm(v3) > vlimit): #limit maximum velocity
                v3 *= vlimit / np.linalg.norm(v3)

            for n in range(N):
                for m in range(N):
                    if m!=n:

                        #Compute vector r from one agent to the next
                        r = p[:,m] - p[:,n] 

                        if r[0] > L/2:
                            r[0] -= L
                        elif r[0] < -L/2:
                            r[0] += L

                        if r[1] > L/2:
                            r[1] -= L
                        elif r[1] < -L/2:
                            r[1] += L
                        #Compute distance between agents rmag

                        rmag = np.sqrt( r[0]**2  + r[1]**2 )
                        #Compute attraction v1
                        v1[:,n] += c1*r 
                        #Compute Repulsion [non-linear scaling] v2
                        
                        v2[:,n] -= (c2 * r) / (rmag**2 )
                        #Compute Repulsion [non-linear scaling] v2
                        #v3 = [ sum ( v ( 1 , : ) ) /N; sum ( v ( 2 , : ) ) /N] ∗ c3 ; %Ali gn a v e r a g e he adin g
                   
                v4[:,n] = c4 * np.random.randn(2) #Compute random velocity component v4
                
                #v5 = c4 * np.random.randn(2,1)
                #print(v4.shape)
                #print(v5.shape)                
                
                v[:,n] = v1[:,n] + v2[:,n] + v3 + v4[:,n] #+ v5 #Update velocity

                
            for n in range(0, N):    
                p[:,n] +=  v[:,n] * delta #Update position

            #Periodic boundary

            tmp_p = p
            tmp_p[0, p[0,:]>L/2] = tmp_p[0,p[0,:]> (L/2)] - L
            tmp_p[1, p[1,:] > L/2] = tmp_p[1, p[1,:] > (L/2)] - L
            tmp_p[0, p[0,:] < -L/2]  = tmp_p[0, p[0,:] < (-L/2)] + L
            tmp_p[1, p[1,:] < -L/2]  = tmp_p[1, p[1,:] < (-L/2)] + L
            p = tmp_p

            # Can Also be written as:
            # p[p > limit] -= limit * 2
            # p[p < -limit] += limit * 2

            line1, = ax.plot(p[0, 0], p[1, 0])

            #update plot
            #ax.clear()
            ax.quiver(p[0,:], p[1,:], v[0,:], v[1,:]) # For drawing velocity arrows
            plt.xlim(-limit, limit)
            plt.ylim(-limit, limit)
            line1.set_data(p[0,:], p[1,:])

            #plt.show()
            fig.canvas.draw()
            plt.savefig('boids' + str(i) +'.png')
            


# def plot_quiver(timesteps, radii):
    
#     line1, = ax.plot(p[0, 0], p[1, 0])

#     ax.quiver(p[0,:], p[1,:], v[0,:], v[1,:]) # For drawing velocity arrows
#     plt.xlim(-limit, limit)
#     plt.ylim(-limit, limit)
#     line1.set_data(p[0,:], p[1,:])


#     fig.canvas.draw()


    
#     plt.ylim(top=100, bottom=0)
#     #plt.plot(radii, color="red", label="25")
#     #plt.plot(radii, color="orange", label="10")
#     plt.plot(radii, color="blue", alpha=0.25, label="25")
    
#     plt.title("Radius of Gyration, LJ Potential = 0.5")
#     plt.ylabel("Radius")
#     plt.xlabel("Time")
#     plt.show()
    
#     return

flock_py = flock()
flock_py.flocking_python()


