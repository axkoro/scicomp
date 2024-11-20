# TODO: Noch nicht die Musterlösung

import numpy as np
import matplotlib.pyplot as plt
import imageio.v2 as imageio
import io

# Config

delta_t = 1.0/200.0
grid_size = 50 # quadratisches Grid

def f(x, y, t):
    return 5*x*t + y

def g(x, y, t):
    if 0.4 <= x <= 0.6 and y > 0.9:
        return 3*t/2
    else:
        return (3*t*np.sin(np.pi*(x + 0.5)) + 
            (1-t)*np.sin(np.pi*(x - 0.5)) + 
            2*np.cos(np.pi*y))
    

# Variablen

num_time_grid_points = (int) (1.0/delta_t) + 1 # N steps <-> N+1 grid points
time_grid = np.linspace(0, 1, num_time_grid_points)

xy_grid = np.linspace(0, 1, grid_size)
delta_xy = xy_grid[1]

num_boundary_values = 4 * grid_size - 4
num_inner_values = (grid_size-2)**2


# Hilfsfunktionen

def get_inner_vector_index(x, y):
    return (x-1) * (grid_size-2) + (y-1)

def generate_g_vector(t):
    vector = np.zeros(num_boundary_values)

    for i in range(grid_size): # Top & bottom boundaries 
        vector[i] = g(0, xy_grid[i], t) # top row

        last_row_index = num_boundary_values - grid_size
        vector[last_row_index + i] = g(grid_size-1, xy_grid[i], t) # bottom row

    for i in range(1, grid_size-1): # Left & right boundaries
        row_index = grid_size + (i-1) # grid_size -> offset from first loop

        vector[row_index] = g(i, 0, t)
        vector[row_index+1] = g(i, grid_size-1, t)
    return vector

def generate_f_vector(t):
    vector = np.zeros(num_inner_values)

    for x in range(1, grid_size-1):
        for y in range(1, grid_size-1):
            index = get_inner_vector_index(x, y)
            vector[index] = f(x, y, t)

    return vector

def generate_A_matrix():
    size = grid_size - 2

    # Submatrix 1 (vgl. T aus der VL)
    T_mat = np.zeros((size, size))
    np.fill_diagonal(T_mat, 4)
    np.fill_diagonal(T_mat[:-1, 1:], -1)  # Upper diagonal
    np.fill_diagonal(T_mat[1:, :-1], -1)  # Lower diagonal

    # Tridiagonal matrix with 4 on diagonal and -1 on off-diagonals
    A_mat = np.kron(np.eye(size), T_mat)

    # Submatrix 2 (-I)
    neg_I_mat = np.zeros((size, size))
    np.fill_diagonal(neg_I_mat, -1)

    # Matrix with 1 on the off-diagonals
    neg_Is_mat = np.zeros((size, size))
    np.fill_diagonal(neg_Is_mat[:-1, 1:], 1)
    np.fill_diagonal(neg_Is_mat[1:, :-1], 1)

    neg_Is_mat = np.kron(neg_Is_mat, neg_I_mat)

    A_mat += neg_Is_mat

    return A_mat

def generate_R_matrix():
    R_mat = np.zeros((num_inner_values, num_boundary_values))

    inner_grid_size = grid_size-2
    
    for i in range(1, inner_grid_size+1):
        # Top & bottom rows
        R_mat[get_inner_vector_index(1, i), i] = 1
        R_mat[get_inner_vector_index(inner_grid_size, i), i] = 1

        # Left & right rows
        R_mat[get_inner_vector_index(i, 1), i] = 1
        R_mat[get_inner_vector_index(i, inner_grid_size), i] = 1

    return R_mat

def generate_full_grid(inner_vector, boundary_vector):
    full_grid = np.zeros((grid_size, grid_size))

    # Fill inner grid values
    inner_grid_size = grid_size - 2
    full_grid[1:-1, 1:-1] = inner_vector.reshape((inner_grid_size, inner_grid_size))
    
    # Fill top boundary
    top_boundary = boundary_vector[:grid_size]
    full_grid[0, :] = top_boundary
    
    # Fill bottom boundary
    bottom_boundary = boundary_vector[-grid_size:]
    full_grid[-1, :] = bottom_boundary

    # Fill left & right boundaries
    left_boundary = boundary_vector[grid_size : -grid_size : 2]
    full_grid[1:-1, 0] = left_boundary

    right_boundary = boundary_vector[grid_size + 1 : -grid_size : 2]
    full_grid[1:-1, -1] = right_boundary

    return full_grid

def main():
    frames = []

    A_matrix = generate_A_matrix()
    R_matrix = generate_R_matrix()

    for t in time_grid:
        # Solve
        g_vector = generate_g_vector(t)
        f_vector = generate_f_vector(t)

        b_vec = R_matrix @ g_vector + delta_xy**2 * f_vector
        u_vector = np.linalg.solve(A_matrix, b_vec)
        full_grid = generate_full_grid(u_vector, g_vector)

        # Plot
        fig, ax = plt.subplots(figsize=(6, 5)) 
        cax = ax.imshow(full_grid, cmap='jet', origin='lower', vmin=-5, vmax=5)
        ax.set_title(f'Time Step: {t:.2f}')
        fig.colorbar(cax)

        buf = io.BytesIO()
        plt.savefig(buf, format='png')
        buf.seek(0)
        frames.append(imageio.imread(buf))
        plt.close(fig)

    imageio.mimsave('result.gif', frames, duration=0.5)

if __name__ == "__main__":
    main()