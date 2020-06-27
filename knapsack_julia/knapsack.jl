using JuMP, GLPK
import MathOptInterface
const MOI = MathOptInterface

if(length(ARGS) < 1)
    println("Expected: julia knapsack.jl [instance filepath]")
    exit()
end

const filePath = ARGS[1]

struct Item
    value::Int64
    weight::Int64
end

items = Item[]
knapCap = open(filePath) do file
    n = parse(Int, readline(file))
    knapCap = parse(Int, readline(file))

    for i in 1:n
        v = parse(Int, readline(file))
        w = parse(Int, readline(file))

        push!(items, Item(v, w))
    end

    (knapCap)
end

function solve(items::Array{Item}, knapCap::Int)
    n = length(items)

    model = Model(GLPK.Optimizer)
    
    # Variables
    @variable(model, x[1:n], Bin)

    # Objective Funcion
    @objective(model, Max, sum(items[i].value*x[i] for i in 1:n))

    # Constraints
    @constraint(model, sum(items[i].weight*x[i] for i in 1:n) <= knapCap)

    # Solve!
    JuMP.optimize!(model)

    # Print solution
    println("Took items:")
    for i in 1:n
        if (value(x[i]) >= 1 - eps())
            print(i, ' ')
        end
    end

    println("\nTotal value: ", JuMP.objective_value(model))
    JuMP.termination_status(model) == MOI.OPTIMAL ? println("Optimal") : println("Not optimal")

    @show JuMP.has_values(model)
    @show JuMP.primal_status(model) == MOI.FEASIBLE_POINT

    @show JuMP.objective_value(model)

    return
end

solve(items, knapCap)