#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
        if(num_nodes <= 1)
            return;
        Vector2D dv = (end-start)/(num_nodes-1);
        for(int i=0; i<num_nodes; ++i)
        {
            Mass* m = new Mass(start+dv*i, node_mass, false);
            masses.push_back(m);
            if(i == 0)
                continue;
            Spring* s = new Spring(masses[i-1], m, k);
            springs.push_back(s);
        }
        // Comment-in this part when you implement the constructor
       for (auto &i : pinned_nodes) {
           masses[i]->pinned = true;
       }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D ab = s->m2->position - s->m1->position;
            Vector2D f = s->k * (ab)*(ab.norm() - s->rest_length)/(ab.norm());
            s->m1->forces += f;
            s->m2->forces += -f;
            // add internal damping force
            float k_d = 0.005;
            Vector2D velocity_ba = s->m2->velocity - s->m1->velocity;
            Vector2D f_d = k_d * dot(ab/ab.norm(), velocity_ba) * ab/ab.norm();
            s->m1->forces += f_d;
            s->m2->forces += -f_d;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += gravity * m->mass;
                
                // TODO (Part 2): Add global damping
                float k_d_global = 0.01;
                m->forces += - k_d_global * m->velocity;
                Vector2D a = m->forces / m->mass;
                //semi-implicit method
                m->velocity += a * delta_t;
                m->position += m->velocity * delta_t;
 
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
       for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D ab = s->m2->position - s->m1->position;
            Vector2D f = s->k *  (ab / ab.norm()) * (ab.norm() - s->rest_length);
            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                m->forces += gravity * m->mass;
                Vector2D a = m->forces / m->mass;

                // TODO (Part 3.1): Set the new position of the rope mass
                Vector2D lastposition = m->position;
                
                // TODO (Part 4): Add global Verlet damping
                float dampfactor = 0.00005;
                m->position = m->position +  (1 - dampfactor) * (m->position - m->last_position) + a * delta_t *delta_t;
                m->last_position = lastposition;
            }
            m->forces =  Vector2D(0,0);
        }
    }
}
