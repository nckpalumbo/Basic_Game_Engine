#include "Model.h"

Model::Model()
{
	vertArr = 0;
	vertCount = 0;
	vertBuf = 0;
}

Model::~Model()
{
}

bool Model::buffer(std::string objFile)
{
	//Vector collections
	vector<glm::vec3> locations;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<VertInd> vertInds;

	//Read in file
	std::ifstream inFile;
	string line;

	inFile.open(objFile);

	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			std::stringstream ss(line);
			std::string line;
			ss >> line;

			if (line == "v")
			{
				float nums[3];
				ss >> nums[0] >> nums[1] >> nums[2];
				locations.push_back(glm::vec3(nums[0], nums[1], nums[2]));
			}
			else if (line == "vt")
			{
				float nums[2];
				ss >> nums[0] >> nums[1];
				uvs.push_back(glm::vec2(nums[0], nums[1]));
			}
			else if (line == "vn")
			{
				float nums[3];
				ss >> nums[0] >> nums[1] >> nums[2];
				normals.push_back(glm::vec3(nums[0], nums[1], nums[2]));
			}
			else if (line == "f")
			{
				unsigned int ints[3];
				char chrs[2];
				for (int i = 0; i < 3; i++)
				{
					ss >> ints[0] >> chrs[0] >> ints[1] >> chrs[1] >> ints[2];
					ints[0]--;
					ints[1]--;
					ints[2]--;
					vertInds.push_back({ints[0], ints[1], ints[2]});
				}
			}
		}
	}
	inFile.close();

	/*vector<Vertex> locs =
	{
		{ { 1,1,0 },{ 1,1 },{0,0,1}},
		{ { -1,1,0 },{ 0,1 },{0,0,1}},
		{ { -1,-1,0 },{ 0,0 },{ 0,0,1}},
		{ { 1,-1,0 },{ 1,0 },{ 0,0,1}}
	};*/

	vertCount = vertInds.size();
	vertBuffData = vector<Vertex>(vertCount);

	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBuffData[i] = { locations[vertInds[i].locInd],
							uvs[vertInds[i].uvInd],
							normals[vertInds[i].normInd] };
	}

	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	//buffer model
	//model loading before game loop
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount, &vertBuffData[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

	glClearColor(1, 1, 1, 0);

	return true;
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
