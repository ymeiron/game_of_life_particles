Game of life – particles
=======================

My background is in *N*-body simulations, but I find cellular automata fascinating. These two classes of simulations have a lot in common, but fundamental differences. The names give away what the main difference is: cellular automata consist of cells (arranged in some regular grid), while *N*-body simulations consist of bodies (particles). Some years ago a particle-based "[Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)" became popular on the [Internet](https://www.youtube.com/watch?v=Z_zmZ23grXE). While it's a cool project, it is in fact just an *N*-body simulation, with individual particles exerting forces on each other depending on distance and the particles' classes. It is not faithful to Conway's original idea for this game, that state (or class) changes depending on the neighbourhood.

<!--more-->

My goal here is not to create a game or simulation rules that produce the most interesting results, the most elaborate and impressive looking "artificial life", but rather find the simplest extension of the original Game of Life to a particle world, and explore if that's still interesting at all.

The original Game has a grid of cells, each one in the "on" or "off" state. At every step of the game, the state of a cell may change depending on the states of its neighbours. It turns *on* if it's *off* and has three *on* neighbours. It turns *off* if it's *on* and has exactly one, or more than three *on* neighbours. These rules can easily be applied to particles, but one has to clarify what "neighbour" means. In a rectangular grid, a cell has exactly eight neighbours. If we imagine the centre of each cell as a stationary particle (that similarly can have one of two states), four of the eight neighbours (up, down, left, right) would be a distance of unity away (that is, one step on the grid). The four other neighbours would be $\sqrt{2}$ units away diagonally. The closest non-neighbours would be two units away. 

So in a particle-based world the rules are applied for each particle where the neighbour search radius is some number between $\sqrt{2}$ and 2. Let's choose the average of these two values, $1+1/\sqrt(2) \approx 1.71$

The initial conditions are some distribution of particles (with velocities), each in one of the two states. The rules on how to move the particles can be absolutely anything, they can (but don't have to) depend on the particles' states. When the particles happen to be arranged on a rectangular grid with spacing of 1, with zero velocities, **we get back the original Game of Life**.

So we have significantly more freedom now, which is not necessarily a good thing, since we want to find interesting patterns and the space of possibilities to explore is really big.

A reasonable starting point would be to have stationary particles like in the original Game, but have them randomly and uniformly distributed. The freedom now is somewhat constrained to how densely we fill the space, what fraction of the particles are *on* and at what pattern, and the random seed used to generate the distribution (that's still quite a lot). Intuitively, I thought that for it to work well, the density should be about one particle per unit area as in the original Game. Interestingly, this gives games that tend to end very quickly, in just a few steps, pretty much independently of the fraction of *on* particles. They end with all particles *off*.

Here for example, the particle density is 2.25 per unit area. What we're seeing is the *on* particles. The *off* particles then form a kind of dark matter medium. In this 400×400 space there are 360k initially *off* particles randomly and uniformly distributed, and on top of them I added 100 initially *on* particles, these are initially on a straight line along the x-axis with a separation of 1. This is quite arbitrary, but the initial distribution of *on* particles doesn't seem to matter that much in this kind of setup.

<p style="color: #A00; font-style: italic">Apologies for the poor quality of the videos; I'm writing this up quite a bit after finishing up with this project, and honestly not overly motivated to spend time on visualizing this.</p>

<div style="width: 300px; margin: auto">
  <video controls loop="0" preload="metadata" style="width: 100%">
    <source type="video/ogg" src="videos/golp-2250.mp4">
    <source type="video/ogg" src="videos/golp-2250.ogv">
  </video>
</div>

Even though this is quite a bit denser than the original Game of Life grid, most initial conditions fizzle out in just a few steps. This is an example of a particularly long game actually. 

Increasing the density up to 2.5 unit, we start getting more long-living games. I stopped this at 2000 steps, I have no idea how long it would have gone. Looks like it got close to extinction at some point but bounced back. At and around this density value we see more interesting behaviour, with some stable and repeating patterns. Here is how that looks like:

<div style="width: 300px; margin: auto">
  <video controls loop="0" preload="metadata" style="width: 100%">
    <source type="video/ogg" src="videos/golp-2500.mp4">
    <source type="video/ogg" src="videos/golp-2500.ogv">
  </video>
</div>

If we increase density further we get out of this interesting "sweet spot". Almost all initial conditions end up filling the space. The video below shows density of 2.75. The "holes" or "voids" here seem to be quite interesting.

<div style="width: 300px; margin: auto">
  <video controls loop="0" preload="metadata" style="width: 100%">
    <source type="video/ogg" src="videos/golp-2750.mp4">
    <source type="video/ogg" src="videos/golp-2750.ogv">
  </video>
</div>

---

The next step is to let the particles move. So I'm adding random velocities, where each velocity component is derived from a normal distribution with some standard deviation *σ*. All particles move at the simulation step, regardless of which state they are in. The boundaries are periodic, so particles "exiting" the space from the right will come back in from the left (etc.)

A bit surprising is that by adding just a little bit of velocity, there is lower density required for a long-lasting game. Here for example the density is 2 units and the velocity dispersion 0.4 (length units per time step); that seems to be around the minimum needed for the game not to fizzle out too quickly at this density.

<div style="width: 300px; margin: auto">
  <video controls loop="0" preload="metadata" style="width: 100%">
    <source type="video/ogg" src="videos/golp-velocities.mp4">
    <source type="video/ogg" src="videos/golp-velocities.ogv">
  </video>
</div>

TL;DR and conclusions
---------------------

This is a Game of Life with particles, a simple extension of the original Game. The original Game is a special case where particles are stationary and laid out on a grid.

We can still achieve long-lasting and curious looking behaviours with random initial conditions, but we don't get the beautiful and symmetric patterns observed in the original Game. The entropy is just too high for that.

That's it for now. The possibilities to extend this are endless even with keeping the state-change rules the same, the rules of motion can be anything. The beauty of the original Game of Life though is that despite its extreme simplicity, it can produce such amazing patterns. I might come back to it in the future if I have any bright idea of how to make it interesting. The code is available [here](https://github.com/ymeiron/game_of_life_particles).