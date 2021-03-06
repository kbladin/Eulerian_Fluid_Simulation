#include <FluidSolver.h>
#include <FluidDomain.h>
#include <Renderer.h>
#include <Canvas.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>

static const int GRID_X_SIZE = 64;
static const int GRID_Y_SIZE = 64;
static const int WORLD_X_SIZE = 1;
static const int WORLD_Y_SIZE = 1;
static const double DELTA_X = WORLD_X_SIZE / double(GRID_X_SIZE);
static const double DELTA_Y = WORLD_Y_SIZE / double(GRID_Y_SIZE);

int main(int argc, char const *argv[])
{
	// Initialize
	FluidDomain fluid_domain(GRID_X_SIZE, GRID_Y_SIZE, WORLD_X_SIZE, WORLD_Y_SIZE, 0.01);
    FluidSolverMemoryPool mem_pool(fluid_domain);
	FluidSolver fluid_solver(mem_pool);
	Renderer renderer({0, WORLD_X_SIZE, 0, WORLD_Y_SIZE});
	Canvas canvas(WORLD_X_SIZE * 512, WORLD_Y_SIZE * 512);

	// Setup
    fluid_domain.addFluidSource(FluidSource( { 2.0 / GRID_X_SIZE, 0.35, 2.0 / GRID_Y_SIZE, 1 - 2.0 / GRID_Y_SIZE }, DELTA_X, DELTA_Y, 0.0, 0.0, 0.0, 1));
    //fluid_domain.addFluidSource(FluidSource( { 1.0 / GRID_X_SIZE, 0.2 }, DELTA_X, DELTA_Y, 0.1, 0.4, 1.5, 5));
    //fluid_domain.addFluidSource(FluidSource( { 0.8, 1.0 - 1.0 / GRID_Y_SIZE, 0.1, 0.4 }, DELTA_X, DELTA_Y, 0.0, 0.0, 1.5, 5));
    //fluid_domain.addFluidSource(FluidSource( { 0.8, 1.2, 0.1, 0.5 }, DELTA_X, DELTA_Y, 0.0, 0.0, 2.0, 2));
    
    //fluid_domain.addFluidSource(FluidSource( { 1.0 / GRID_X_SIZE, 2.0 / GRID_X_SIZE, 0.3, 0.35 }, DELTA_X, DELTA_Y, 3.0, 0.0, 0.003, 300));
    //fluid_domain.addFluidSource(FluidSource( { 1.0 - 2.0 / GRID_X_SIZE, 1.0 - 1.0 / GRID_X_SIZE, 0.3, 0.35 }, DELTA_X, DELTA_Y, -3.0, 0.0, 0.003, 300));
    //fluid_domain.addFluidSource(FluidSource( { 0.17, 0.23, 1.0 / GRID_Y_SIZE, 2.0 / GRID_Y_SIZE }, DELTA_X, DELTA_Y, 0.0, 3.0, 0.003, 300));
    //fluid_domain.addFluidSource(FluidSource( { 1-0.23, 1-0.17, 1.0 / GRID_Y_SIZE, 2.0 / GRID_Y_SIZE }, DELTA_X, DELTA_Y, 0.0, 3.0, 0.003, 300));
    
    // Prepare simulation
    int n_frames = 500;
	MyFloat seconds_per_frame = 0.02;
	time_t time_start, time_end;
	time(&time_start);

	// Start simulation
	for (int i = 0; i < n_frames; ++i)
	{
		std::cout << "frame " << i << std::endl;

		MyFloat dt;
		// Simulate
		for (MyFloat frame_time = 0; frame_time < seconds_per_frame; frame_time += dt)
		{
			// Calculate dt (for now just set it)
			dt = 0.01;
            dt = CLAMP(dt, 0, seconds_per_frame - frame_time);

			// Update fluid domain (creates new fluid from sources)
			fluid_domain.update(dt);

			// Solve
			try
			{
				//fluid_solver.stepSemiLagrangian(fluid_domain, dt);
				fluid_solver.stepPICFLIP(fluid_domain, dt, 0.05);
			}
			catch (const std::runtime_error& e)
			{
				std::cout << e.what() << std::endl;
				return EXIT_FAILURE;
			}
		}
		// Render
		renderer.clearCanvas(canvas);
		renderer.renderGridCellsToCanvas(fluid_domain.macGrid(), canvas);
		//renderer.renderMetaBallsToCanvas(fluid_domain.markerParticleSet(), canvas);
		renderer.renderParticlesToCanvas(fluid_domain.markerParticleSet(), canvas);
		//renderer.renderGridVelocitiesToCanvas(fluid_domain.macGrid(), canvas);
		
		std::stringstream file_name;
        file_name << std::setfill('0') << std::setw(4) << i << ".ppm";
		renderer.writeCanvasToPpm(file_name.str().c_str(), canvas);
	}
	time(&time_end);

	double time_elapsed = difftime(time_end, time_start);

	int hours_elapsed = time_elapsed / (60 * 60);
	int minutes_elapsed = (int(time_elapsed) % (60 * 60)) / 60;
	int seconds_elapsed = int(time_elapsed) % 60;

	std::string elapsed_time_string =
	 	  std::to_string(hours_elapsed) + "h:"
		+ std::to_string(minutes_elapsed) + "m:"
		+ std::to_string(seconds_elapsed) + "s";

	std::cout << "Simulation time : " << elapsed_time_string << std::endl;

	return EXIT_SUCCESS;
}
