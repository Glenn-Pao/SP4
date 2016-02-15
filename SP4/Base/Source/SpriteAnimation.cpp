#include "SpriteAnimation.h"
#include "GL\glew.h"
#include "Vertex.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
 : Mesh(meshName)
 , m_row(row)
 , m_col(col)
 , m_currentTime(0)
 , m_currentFrame(0)
 , m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if(m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
{
	if(m_anim) // If animation happens
	{
		// Animation current time, m_currentTime increments by delta time, dt
		m_currentTime += dt;

		// Variable int numFrame is set to maximum value between (1, (endFrame – startFrame + 1))
		int numFrame = Max(1, (m_anim->endFrame - m_anim->startFrame + 1));
		// Variable float frameTime = animTime / numFrame
		float frameTime = m_anim->animTime / numFrame;
		// Animation current frame, m_currentFrame is set to the minimum value between (endFrame, (startFrame + (m_currentTime/ frameTime)))
		m_currentFrame = Min(m_anim->endFrame, (m_anim->startFrame + (m_currentTime/ frameTime)));
		// If the m_currentTime is greater than animTime
		//Set animation end flag, ended to true
		if(m_currentTime > m_anim->animTime)
		{
			m_anim->ended = true;
			//If m_playCount + 1 is less than repeatCount
			//or repeatCount == 0
			if(m_playCount + 1 < m_anim->repeatCount || m_anim->repeatCount == 0)
			{
				//then increment m_playCount
				++m_playCount;
				//reset m_currentTime
				m_currentTime = 0.f;
				//set m_currentFrame = startFrame
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}

void SpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	
	//if(texCount > 0)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}

	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6  * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//if(texCount > 0)
	{
		glDisableVertexAttribArray(3);
	}
}